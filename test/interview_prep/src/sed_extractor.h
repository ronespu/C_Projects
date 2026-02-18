#ifndef SED_EXTRACTOR_H
#define SED_EXTRACTOR_H

#include <stddef.h>

#define SED_MAX_FFT 512
#define SED_MAX_BINS ((SED_MAX_FFT / 2) + 1)
#define SED_MEL_BANDS 8

typedef struct {
    float rms;
    float zcr;
    float spectral_centroid_hz;
    float spectral_flux;
    float mel_log_energy[SED_MEL_BANDS];
} sed_features_t;

typedef struct {
    size_t fft_size;
    float sample_rate_hz;
    float prev_mag[SED_MAX_BINS];
    int has_prev;
} sed_extractor_t;

int sed_extractor_init(sed_extractor_t *ctx, size_t fft_size, float sample_rate_hz);
int sed_extractor_process(sed_extractor_t *ctx, const float *frame, sed_features_t *features);

#endif
