#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <stddef.h>

typedef enum {
    DIAG_OK = 0,
    DIAG_XRUN = 1,
    DIAG_CLIP = 2,
    DIAG_OVERRUN = 3
} diag_event_t;

typedef struct {
    unsigned long xrun_count;
    unsigned long clip_count;
    unsigned long overrun_count;
    float peak_abs;
    float avg_process_ms;
    unsigned long frames;
} diag_state_t;

void diag_init(diag_state_t *d);
void diag_mark_event(diag_state_t *d, diag_event_t e);
void diag_update_timing(diag_state_t *d, float process_ms);
void diag_update_peak(diag_state_t *d, float sample);
void diag_print_summary(const diag_state_t *d);

#endif
