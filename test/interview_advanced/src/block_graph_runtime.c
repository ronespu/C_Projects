#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define N 256

typedef struct {
    float preemph;
    float gain;
    int bypass;
} dsp_cfg_t;

static float pre_emphasis(float x, float prev, float a)
{
    return x - a * prev;
}

static float run_block_graph(float in, float *prev, const dsp_cfg_t *cfg)
{
    float y = in;

    if (!cfg->bypass) {
        y = pre_emphasis(in, *prev, cfg->preemph);
        y *= cfg->gain;
    }
    *prev = in;
    return y;
}

int main(void)
{
    dsp_cfg_t cfg = {0.95f, 1.4f, 0};
    float prev = 0.0f;
    int i;

    printf("graph: input -> preemphasis -> gain -> output (runtime tunable)\n");

    for (i = 0; i < N; ++i) {
        float t = (float)i / 16000.0f;
        float in = 0.6f * sinf(2.0f * (float)M_PI * 500.0f * t);
        float out;

        if (i == 120) {
            cfg.gain = 0.8f;
        }
        if (i == 200) {
            cfg.bypass = 1;
        }

        out = run_block_graph(in, &prev, &cfg);
        if (i % 64 == 0) {
            printf("i=%d gain=%.2f bypass=%d in=%.4f out=%.4f\n", i, cfg.gain, cfg.bypass, in, out);
        }
    }

    return 0;
}
