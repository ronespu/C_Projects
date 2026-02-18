#include "fft.h"

#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int is_pow2(size_t n)
{
    return n && ((n & (n - 1U)) == 0U);
}

static unsigned rev_bits(unsigned x, unsigned bits)
{
    unsigned y = 0U;
    unsigned i;
    for (i = 0U; i < bits; ++i) {
        y = (y << 1U) | (x & 1U);
        x >>= 1U;
    }
    return y;
}

int fft_real_forward(const float *in, size_t n, cpxf_t *out_half)
{
    cpxf_t *buf;
    unsigned bits = 0U;
    size_t i;
    size_t len;

    if (!in || !out_half || !is_pow2(n)) {
        return -1;
    }

    while ((1U << bits) < n) {
        bits++;
    }

    buf = (cpxf_t *)malloc(sizeof(cpxf_t) * n);
    if (!buf) {
        return -1;
    }

    for (i = 0U; i < n; ++i) {
        unsigned j = rev_bits((unsigned)i, bits);
        buf[j].re = in[i];
        buf[j].im = 0.0f;
    }

    for (len = 2U; len <= n; len <<= 1U) {
        size_t half = len >> 1U;
        float ang_step = -2.0f * (float)M_PI / (float)len;
        size_t base;
        for (base = 0U; base < n; base += len) {
            size_t k;
            for (k = 0U; k < half; ++k) {
                float ang = ang_step * (float)k;
                float wr = cosf(ang);
                float wi = sinf(ang);
                cpxf_t u = buf[base + k];
                cpxf_t v = buf[base + k + half];
                float tr = wr * v.re - wi * v.im;
                float ti = wr * v.im + wi * v.re;
                buf[base + k].re = u.re + tr;
                buf[base + k].im = u.im + ti;
                buf[base + k + half].re = u.re - tr;
                buf[base + k + half].im = u.im - ti;
            }
        }
    }

    for (i = 0U; i <= n / 2U; ++i) {
        out_half[i] = buf[i];
    }

    free(buf);
    return 0;
}
