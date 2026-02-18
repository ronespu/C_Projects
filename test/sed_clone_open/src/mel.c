#include "mel.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

static float hz_to_mel(float hz)
{
    return 2595.0f * log10f(1.0f + (hz / 700.0f));
}

static float mel_to_hz(float mel)
{
    return 700.0f * (powf(10.0f, mel / 2595.0f) - 1.0f);
}

int mel_build_filterbank(int sample_rate_hz,
                         int fft_size,
                         int n_mels,
                         float fmin_hz,
                         float fmax_hz,
                         mel_filterbank_t *fb)
{
    int *mel_bins;
    float mel_min;
    float mel_max;
    int i;
    int total = 0;

    if (!fb || sample_rate_hz <= 0 || fft_size <= 0 || n_mels <= 0 || fmax_hz <= fmin_hz) {
        return -1;
    }

    memset(fb, 0, sizeof(*fb));
    fb->n_mels = n_mels;
    fb->n_bins = (fft_size / 2) + 1;
    fb->bands = (mel_band_t *)calloc((size_t)n_mels, sizeof(mel_band_t));
    if (!fb->bands) {
        return -1;
    }

    mel_bins = (int *)calloc((size_t)(n_mels + 2), sizeof(int));
    if (!mel_bins) {
        mel_free_filterbank(fb);
        return -1;
    }

    mel_min = hz_to_mel(fmin_hz);
    mel_max = hz_to_mel(fmax_hz);
    for (i = 0; i < n_mels + 2; ++i) {
        float mel = mel_min + ((float)i * (mel_max - mel_min) / (float)(n_mels + 1));
        float hz = mel_to_hz(mel);
        int bin = (int)lroundf((float)fft_size * hz / (float)sample_rate_hz);
        if (bin < 0) {
            bin = 0;
        }
        if (bin >= fb->n_bins) {
            bin = fb->n_bins - 1;
        }
        mel_bins[i] = bin;
    }

    for (i = 0; i < n_mels; ++i) {
        int l = mel_bins[i];
        int c = mel_bins[i + 1];
        int r = mel_bins[i + 2];
        int start = l;
        int end = r;
        if (start < 0) {
            start = 0;
        }
        if (end >= fb->n_bins) {
            end = fb->n_bins - 1;
        }

        fb->bands[i].start_bin = start;
        fb->bands[i].len = (end >= start) ? (end - start + 1) : 0;
        fb->bands[i].off = total;
        total += fb->bands[i].len;

        if (c <= l) {
            c = l + 1;
        }
        if (r <= c) {
            r = c + 1;
        }
    }

    fb->weights = (float *)calloc((size_t)total, sizeof(float));
    if (!fb->weights) {
        free(mel_bins);
        mel_free_filterbank(fb);
        return -1;
    }
    fb->total_weights = total;

    for (i = 0; i < n_mels; ++i) {
        int l = mel_bins[i];
        int c = mel_bins[i + 1];
        int r = mel_bins[i + 2];
        int b;

        if (c <= l) {
            c = l + 1;
        }
        if (r <= c) {
            r = c + 1;
        }

        for (b = fb->bands[i].start_bin; b < fb->bands[i].start_bin + fb->bands[i].len; ++b) {
            float w;
            if (b < c) {
                w = (float)(b - l) / (float)(c - l);
            } else {
                w = (float)(r - b) / (float)(r - c);
            }
            if (w < 0.0f) {
                w = 0.0f;
            }
            fb->weights[fb->bands[i].off + (b - fb->bands[i].start_bin)] = w;
        }
    }

    free(mel_bins);
    return 0;
}

void mel_free_filterbank(mel_filterbank_t *fb)
{
    if (!fb) {
        return;
    }
    free(fb->weights);
    free(fb->bands);
    memset(fb, 0, sizeof(*fb));
}
