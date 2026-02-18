#ifndef MEL_H
#define MEL_H

#include <stddef.h>

typedef struct {
    int start_bin;
    int len;
    int off;
} mel_band_t;

typedef struct {
    int n_mels;
    int n_bins;
    float *weights;
    mel_band_t *bands;
    int total_weights;
} mel_filterbank_t;

int mel_build_filterbank(int sample_rate_hz,
                         int fft_size,
                         int n_mels,
                         float fmin_hz,
                         float fmax_hz,
                         mel_filterbank_t *fb);
void mel_free_filterbank(mel_filterbank_t *fb);

#endif
