#ifndef SED_OPEN_H
#define SED_OPEN_H

#include <stddef.h>
#include <stdint.h>

#include "mel.h"

#define SED_WIN_LEN 4096
#define SED_HOP_LEN 2048
#define SED_HALF_LEN ((SED_WIN_LEN / 2) + 1)
#define SED_N_MELS 40

typedef struct {
    float input_scale;
    int zero_point;
} int8_quant_params_t;

typedef struct {
    float win[SED_WIN_LEN];
    float hann[SED_WIN_LEN];
    size_t fill;
    mel_filterbank_t mel_fb;
    float prev_mag[SED_HALF_LEN];
    int has_prev;
    int sample_rate_hz;
} sed_open_t;

typedef struct {
    float rms;
    float zcr;
    float centroid_hz;
    float flux;
    float logmel[SED_N_MELS];
    int8_t logmel_q[SED_N_MELS];
} sed_frame_features_t;

int sed_open_init(sed_open_t *st, int sample_rate_hz);
void sed_open_deinit(sed_open_t *st);
int sed_open_push(sed_open_t *st,
                  float sample,
                  const int8_quant_params_t *q,
                  sed_frame_features_t *out_feat,
                  int *ready);

#endif
