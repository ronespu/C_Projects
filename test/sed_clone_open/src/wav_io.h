#ifndef WAV_IO_H
#define WAV_IO_H

#include <stddef.h>

typedef struct {
    int sample_rate;
    int channels;
    int bits_per_sample;
} wav_info_t;

int wav_read_mono16(const char *path, float **samples, size_t *count, wav_info_t *info);

#endif
