#include "diagnostics.h"

#include <math.h>
#include <stdio.h>

void diag_init(diag_state_t *d)
{
    if (!d) {
        return;
    }
    d->xrun_count = 0UL;
    d->clip_count = 0UL;
    d->overrun_count = 0UL;
    d->peak_abs = 0.0f;
    d->avg_process_ms = 0.0f;
    d->frames = 0UL;
}

void diag_mark_event(diag_state_t *d, diag_event_t e)
{
    if (!d) {
        return;
    }
    switch (e) {
    case DIAG_XRUN:
        d->xrun_count++;
        break;
    case DIAG_CLIP:
        d->clip_count++;
        break;
    case DIAG_OVERRUN:
        d->overrun_count++;
        break;
    case DIAG_OK:
    default:
        break;
    }
}

void diag_update_timing(diag_state_t *d, float process_ms)
{
    if (!d || process_ms < 0.0f) {
        return;
    }
    d->frames++;
    d->avg_process_ms += (process_ms - d->avg_process_ms) / (float)d->frames;
}

void diag_update_peak(diag_state_t *d, float sample)
{
    float a;
    if (!d) {
        return;
    }
    a = fabsf(sample);
    if (a > d->peak_abs) {
        d->peak_abs = a;
    }
    if (a > 1.0f) {
        diag_mark_event(d, DIAG_CLIP);
    }
}

void diag_print_summary(const diag_state_t *d)
{
    if (!d) {
        return;
    }
    printf("diag: frames=%lu avg_process=%.3fms peak=%.3f xrun=%lu clip=%lu overrun=%lu\n",
           d->frames,
           d->avg_process_ms,
           d->peak_abs,
           d->xrun_count,
           d->clip_count,
           d->overrun_count);
}
