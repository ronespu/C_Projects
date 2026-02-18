#include "sed_open.h"

#include <math.h>
#include <string.h>

#include "fft.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static float safe_logf(float x)
{
    return logf(x + 1e-8f);
}

static int8_t clamp_i8(int x)
{
    if (x > 127) {
        return 127;
    }
    if (x < -128) {
        return -128;
    }
    return (int8_t)x;
}

int sed_open_init(sed_open_t *st, int sample_rate_hz)
{
    size_t i;

    if (!st || sample_rate_hz <= 0) {
        return -1;
    }

    memset(st, 0, sizeof(*st));
    st->sample_rate_hz = sample_rate_hz;
    for (i = 0U; i < SED_WIN_LEN; ++i) {
        st->hann[i] = 0.5f - 0.5f * cosf(2.0f * (float)M_PI * (float)i / (float)(SED_WIN_LEN - 1));
    }

    if (mel_build_filterbank(sample_rate_hz,
                             SED_WIN_LEN,
                             SED_N_MELS,
                             50.0f,
                             (float)(sample_rate_hz / 2),
                             &st->mel_fb) != 0) {
        return -1;
    }

    return 0;
}

void sed_open_deinit(sed_open_t *st)
{
    if (!st) {
        return;
    }
    mel_free_filterbank(&st->mel_fb);
}

static void compute_features(sed_open_t *st,
                             const float *frame,
                             const int8_quant_params_t *q,
                             sed_frame_features_t *out)
{
    cpxf_t spec[SED_HALF_LEN];
    float mag[SED_HALF_LEN];
    float energy = 0.0f;
    int zc = 0;
    size_t i;

    fft_real_forward(frame, SED_WIN_LEN, spec);

    for (i = 0U; i < SED_HALF_LEN; ++i) {
        float re = spec[i].re;
        float im = spec[i].im;
        mag[i] = sqrtf(re * re + im * im);
    }

    for (i = 0U; i < SED_WIN_LEN; ++i) {
        energy += frame[i] * frame[i];
        if (i > 0U) {
            if ((frame[i - 1U] >= 0.0f && frame[i] < 0.0f) ||
                (frame[i - 1U] < 0.0f && frame[i] >= 0.0f)) {
                zc++;
            }
        }
    }

    out->rms = sqrtf(energy / (float)SED_WIN_LEN);
    out->zcr = (float)zc / (float)(SED_WIN_LEN - 1U);

    {
        float sum = 0.0f;
        float wsum = 0.0f;
        for (i = 0U; i < SED_HALF_LEN; ++i) {
            float f = (float)i * (float)st->sample_rate_hz / (float)SED_WIN_LEN;
            sum += mag[i];
            wsum += mag[i] * f;
        }
        out->centroid_hz = (sum > 0.0f) ? (wsum / sum) : 0.0f;
    }

    if (st->has_prev) {
        float flux = 0.0f;
        for (i = 0U; i < SED_HALF_LEN; ++i) {
            float d = mag[i] - st->prev_mag[i];
            if (d > 0.0f) {
                flux += d;
            }
        }
        out->flux = flux / (float)SED_HALF_LEN;
    } else {
        out->flux = 0.0f;
    }

    for (i = 0U; i < SED_HALF_LEN; ++i) {
        st->prev_mag[i] = mag[i];
    }
    st->has_prev = 1;

    for (i = 0U; i < SED_N_MELS; ++i) {
        int b0 = st->mel_fb.bands[i].start_bin;
        int len = st->mel_fb.bands[i].len;
        int off = st->mel_fb.bands[i].off;
        int j;
        float mel_energy = 0.0f;

        for (j = 0; j < len; ++j) {
            float v = mag[b0 + j];
            mel_energy += st->mel_fb.weights[off + j] * v * v;
        }

        out->logmel[i] = safe_logf(mel_energy);
        if (out->logmel[i] < -8.0f) {
            out->logmel[i] = -8.0f;
        }
    }

    if (q) {
        float src_scale = 1.0f / 33554432.0f;
        for (i = 0U; i < SED_N_MELS; ++i) {
            int v = (int)lroundf((out->logmel[i] * src_scale) / q->input_scale) + q->zero_point;
            out->logmel_q[i] = clamp_i8(v);
        }
    }
}

int sed_open_push(sed_open_t *st,
                  float sample,
                  const int8_quant_params_t *q,
                  sed_frame_features_t *out_feat,
                  int *ready)
{
    if (!st || !out_feat || !ready) {
        return -1;
    }

    st->win[st->fill++] = sample;
    *ready = 0;

    if (st->fill == SED_WIN_LEN) {
        float frame[SED_WIN_LEN];
        size_t i;
        for (i = 0U; i < SED_WIN_LEN; ++i) {
            frame[i] = st->win[i] * st->hann[i];
        }

        compute_features(st, frame, q, out_feat);

        memmove(st->win, st->win + SED_HOP_LEN, sizeof(float) * (SED_WIN_LEN - SED_HOP_LEN));
        st->fill = SED_WIN_LEN - SED_HOP_LEN;
        *ready = 1;
    }

    return 0;
}
