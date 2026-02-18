#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sed_open.h"
#include "wav_io.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char **argv)
{
    sed_open_t st;
    int8_quant_params_t q = {0.9999854f, 0};
    sed_frame_features_t feat;
    int ready;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mono16.wav> | --synth\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--synth") == 0) {
        int sr = 16000;
        int n = 3 * sr;
        int i;
        int frame_idx = 0;
        if (sed_open_init(&st, sr) != 0) {
            fprintf(stderr, "Error initializing sed pipeline\n");
            return 3;
        }
        for (i = 0; i < n; ++i) {
            float t = (float)i / (float)sr;
            float s = 0.7f * sinf(2.0f * (float)M_PI * 600.0f * t) +
                      0.3f * sinf(2.0f * (float)M_PI * 1800.0f * t);
            if (sed_open_push(&st, s, &q, &feat, &ready) != 0) {
                sed_open_deinit(&st);
                return 4;
            }
            if (ready && frame_idx < 8) {
                printf("frame=%d rms=%.5f zcr=%.5f centroid=%.1f flux=%.5f logmel0=%.4f q0=%d\n",
                       frame_idx,
                       feat.rms,
                       feat.zcr,
                       feat.centroid_hz,
                       feat.flux,
                       feat.logmel[0],
                       (int)feat.logmel_q[0]);
                frame_idx++;
            }
        }
        sed_open_deinit(&st);
        return 0;
    }

    {
        float *samples = NULL;
        size_t count = 0U;
        wav_info_t info;
        size_t i;
        int frame_idx = 0;

        if (wav_read_mono16(argv[1], &samples, &count, &info) != 0) {
            fprintf(stderr, "Error reading wav file: %s\n", argv[1]);
            return 2;
        }

        if (sed_open_init(&st, info.sample_rate) != 0) {
            free(samples);
            fprintf(stderr, "Error initializing sed pipeline\n");
            return 3;
        }

        for (i = 0U; i < count; ++i) {
            if (sed_open_push(&st, samples[i], &q, &feat, &ready) != 0) {
                free(samples);
                sed_open_deinit(&st);
                return 4;
            }
            if (ready) {
                printf("frame=%d rms=%.5f zcr=%.5f centroid=%.1f flux=%.5f logmel0=%.4f q0=%d\n",
                       frame_idx,
                       feat.rms,
                       feat.zcr,
                       feat.centroid_hz,
                       feat.flux,
                       feat.logmel[0],
                       (int)feat.logmel_q[0]);
                frame_idx++;
            }
        }

        free(samples);
        sed_open_deinit(&st);
    }

    return 0;
}
