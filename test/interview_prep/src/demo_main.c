#include "dsp_control.h"
#include "ring_buffer.h"
#include "sed_extractor.h"

#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FRAME 256

static void print_features(const sed_features_t *f)
{
    int i;
    printf("RMS=%.5f ZCR=%.5f Centroid=%.2fHz Flux=%.5f\n",
           f->rms, f->zcr, f->spectral_centroid_hz, f->spectral_flux);
    printf("log-mel:");
    for (i = 0; i < SED_MEL_BANDS; ++i) {
        printf(" %.3f", f->mel_log_energy[i]);
    }
    printf("\n");
}

int main(void)
{
    float storage[FRAME];
    ring_buffer_t rb;
    dsp_state_t dsp;
    sed_extractor_t ext;
    float frame[FRAME];
    sed_features_t feats;
    size_t i;

    if (rb_init(&rb, storage, FRAME) != 0) {
        return 1;
    }
    dsp_state_init(&dsp);
    if (dsp_apply_command(&dsp, CMD_SET_GAIN, 1.8f) != 0) {
        return 2;
    }
    if (sed_extractor_init(&ext, FRAME, 16000.0f) != 0) {
        return 3;
    }

    for (i = 0U; i < FRAME; ++i) {
        float t = (float)i / 16000.0f;
        float tone1 = 0.7f * sinf(2.0f * (float)M_PI * 500.0f * t);
        float tone2 = 0.3f * sinf(2.0f * (float)M_PI * 1600.0f * t);
        float sample = dsp_process_sample(&dsp, tone1 + tone2);
        rb_push(&rb, sample);
    }

    for (i = 0U; i < FRAME; ++i) {
        if (rb_pop(&rb, &frame[i]) != 0) {
            return 4;
        }
    }

    if (sed_extractor_process(&ext, frame, &feats) != 0) {
        return 5;
    }
    print_features(&feats);

    if (dsp_apply_command(&dsp, CMD_SET_BYPASS, 1.0f) != 0) {
        return 6;
    }
    for (i = 0U; i < FRAME; ++i) {
        float t = (float)i / 16000.0f;
        float sample = sinf(2.0f * (float)M_PI * 500.0f * t);
        rb_push(&rb, dsp_process_sample(&dsp, sample));
    }
    for (i = 0U; i < FRAME; ++i) {
        if (rb_pop(&rb, &frame[i]) != 0) {
            return 7;
        }
    }

    if (sed_extractor_process(&ext, frame, &feats) != 0) {
        return 8;
    }
    print_features(&feats);

    return 0;
}
