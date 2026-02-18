#include <math.h>
#include <stdio.h>
#include <time.h>

#include "diagnostics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FRAME 128
#define SR 16000
#define NUM_FRAMES 40

static float g_buf_a[FRAME];
static float g_buf_b[FRAME];

static float now_ms(void)
{
    return 1000.0f * (float)clock() / (float)CLOCKS_PER_SEC;
}

static void dma_fill(float *buf, int frame_idx)
{
    int i;
    for (i = 0; i < FRAME; ++i) {
        float t = (float)(frame_idx * FRAME + i) / (float)SR;
        buf[i] = 0.8f * sinf(2.0f * (float)M_PI * 1000.0f * t);
    }
}

static void dsp_process(float *buf, diag_state_t *diag)
{
    int i;
    for (i = 0; i < FRAME; ++i) {
        buf[i] *= 1.3f;
        diag_update_peak(diag, buf[i]);
    }
}

int main(void)
{
    int frame;
    float frame_budget_ms = 1000.0f * (float)FRAME / (float)SR;
    diag_state_t diag;
    diag_init(&diag);

    for (frame = 0; frame < NUM_FRAMES; ++frame) {
        float *active = (frame & 1) ? g_buf_b : g_buf_a;
        float t0;
        float t1;
        float dt;

        dma_fill(active, frame);

        t0 = now_ms();
        dsp_process(active, &diag);

        if ((frame % 11) == 0) {
            volatile int spin;
            for (spin = 0; spin < 200000; ++spin) {
            }
        }

        t1 = now_ms();
        dt = t1 - t0;
        diag_update_timing(&diag, dt);

        if (dt > frame_budget_ms) {
            diag_mark_event(&diag, DIAG_OVERRUN);
            printf("frame=%d overrun process=%.3fms budget=%.3fms\n", frame, dt, frame_budget_ms);
        }
    }

    diag_print_summary(&diag);
    return 0;
}
