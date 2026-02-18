#ifndef FFT_H
#define FFT_H

#include <stddef.h>

typedef struct {
    float re;
    float im;
} cpxf_t;

int fft_real_forward(const float *in, size_t n, cpxf_t *out_half);

#endif
