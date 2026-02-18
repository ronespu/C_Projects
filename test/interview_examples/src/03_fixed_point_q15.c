#include <stdint.h>
#include <stdio.h>

#define Q15_ONE 32768

static int16_t float_to_q15(float x)
{
    if (x > 0.999969f) {
        x = 0.999969f;
    }
    if (x < -1.0f) {
        x = -1.0f;
    }
    return (int16_t)(x * (float)Q15_ONE);
}

static int16_t q15_mul(int16_t a, int16_t b)
{
    int32_t p = (int32_t)a * (int32_t)b;
    p += (1 << 14);
    p >>= 15;
    if (p > 32767) {
        p = 32767;
    }
    if (p < -32768) {
        p = -32768;
    }
    return (int16_t)p;
}

int main(void)
{
    int16_t gain = float_to_q15(0.8f);
    int16_t in[8] = {
        float_to_q15(0.1f), float_to_q15(0.2f), float_to_q15(0.3f), float_to_q15(0.4f),
        float_to_q15(-0.1f), float_to_q15(-0.2f), float_to_q15(-0.3f), float_to_q15(-0.4f)
    };
    int i;

    for (i = 0; i < 8; ++i) {
        int16_t out = q15_mul(in[i], gain);
        printf("in_q15=%d out_q15=%d\n", in[i], out);
    }

    printf("Q15 point: deterministic, fast, but scaling/saturation are your responsibility.\n");
    return 0;
}
