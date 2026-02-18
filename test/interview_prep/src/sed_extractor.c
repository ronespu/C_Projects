#include "sed_extractor.h"

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int is_power_of_two(size_t n)
{
    return n && ((n & (n - 1U)) == 0U);
}

int sed_extractor_init(sed_extractor_t *ctx, size_t fft_size, float sample_rate_hz)
{
    if (!ctx || !is_power_of_two(fft_size) || fft_size > SED_MAX_FFT || fft_size < 32U || sample_rate_hz <= 0.0f) {
        return -1;
    }

    ctx->fft_size = fft_size;
    ctx->sample_rate_hz = sample_rate_hz;
    memset(ctx->prev_mag, 0, sizeof(ctx->prev_mag));
    ctx->has_prev = 0;
    return 0;
}

static void compute_magnitude_spectrum(const float *frame, size_t n, float *mag)
{
    size_t k;
    for (k = 0U; k <= n / 2U; ++k) {
        float re = 0.0f;
        float im = 0.0f;
        size_t t;

        for (t = 0U; t < n; ++t) {
            float w = 0.5f - 0.5f * cosf((2.0f * (float)M_PI * (float)t) / (float)(n - 1U));
            float xw = frame[t] * w;
            float angle = (2.0f * (float)M_PI * (float)k * (float)t) / (float)n;
            re += xw * cosf(angle);
            im -= xw * sinf(angle);
        }
        mag[k] = sqrtf((re * re) + (im * im));
    }
}

static float safe_logf(float x)
{
    const float eps = 1e-8f;
    return logf(x + eps);
}

int sed_extractor_process(sed_extractor_t *ctx, const float *frame, sed_features_t *features)
{
    size_t i;
    float mag[SED_MAX_BINS] = {0};
    size_t bins;
    float spec_sum = 0.0f;
    float weighted_sum = 0.0f;
    int zc = 0;
    float energy = 0.0f;

    if (!ctx || !frame || !features) {
        return -1;
    }

    bins = (ctx->fft_size / 2U) + 1U;
    compute_magnitude_spectrum(frame, ctx->fft_size, mag);

    for (i = 0U; i < ctx->fft_size; ++i) {
        float s = frame[i];
        energy += s * s;
        if (i > 0U) {
            if ((frame[i - 1U] >= 0.0f && frame[i] < 0.0f) ||
                (frame[i - 1U] < 0.0f && frame[i] >= 0.0f)) {
                zc++;
            }
        }
    }

    features->rms = sqrtf(energy / (float)ctx->fft_size);
    features->zcr = (float)zc / (float)(ctx->fft_size - 1U);

    for (i = 0U; i < bins; ++i) {
        float freq = ((float)i * ctx->sample_rate_hz) / (float)ctx->fft_size;
        spec_sum += mag[i];
        weighted_sum += mag[i] * freq;
    }
    features->spectral_centroid_hz = (spec_sum > 0.0f) ? (weighted_sum / spec_sum) : 0.0f;

    if (ctx->has_prev) {
        float flux = 0.0f;
        for (i = 0U; i < bins; ++i) {
            float d = mag[i] - ctx->prev_mag[i];
            if (d > 0.0f) {
                flux += d;
            }
        }
        features->spectral_flux = flux / (float)bins;
    } else {
        features->spectral_flux = 0.0f;
    }

    for (i = 0U; i < SED_MEL_BANDS; ++i) {
        size_t start = (i * bins) / SED_MEL_BANDS;
        size_t end = ((i + 1U) * bins) / SED_MEL_BANDS;
        size_t b;
        float band_energy = 0.0f;
        for (b = start; b < end; ++b) {
            band_energy += mag[b] * mag[b];
        }
        features->mel_log_energy[i] = safe_logf(band_energy);
    }

    memcpy(ctx->prev_mag, mag, sizeof(float) * bins);
    ctx->has_prev = 1;
    return 0;
}
