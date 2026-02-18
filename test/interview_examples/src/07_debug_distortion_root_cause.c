#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define N 1024

static float saturate(float x)
{
    if (x > 1.0f) {
        return 1.0f;
    }
    if (x < -1.0f) {
        return -1.0f;
    }
    return x;
}

int main(void)
{
    float gain = 2.2f;
    int clipped = 0;
    float peak_in = 0.0f;
    float peak_out = 0.0f;
    int i;

    for (i = 0; i < N; ++i) {
        float t = (float)i / 16000.0f;
        float in = 0.7f * sinf(2.0f * (float)M_PI * 1000.0f * t);
        float out_ideal = in * gain;
        float out = saturate(out_ideal);

        if (fabsf(in) > peak_in) {
            peak_in = fabsf(in);
        }
        if (fabsf(out) > peak_out) {
            peak_out = fabsf(out);
        }
        if (out != out_ideal) {
            clipped++;
        }
    }

    printf("peak_in=%.3f peak_out=%.3f gain=%.2f clipped_samples=%d/%d\n",
           peak_in, peak_out, gain, clipped, N);
    printf("Root cause path: distortion -> check peak levels -> detect clipping -> tune gain/headroom.\n");
    return 0;
}
