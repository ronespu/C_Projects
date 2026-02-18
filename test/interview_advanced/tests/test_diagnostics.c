#include <assert.h>

#include "diagnostics.h"

int main(void)
{
    diag_state_t d;

    diag_init(&d);
    diag_mark_event(&d, DIAG_XRUN);
    diag_mark_event(&d, DIAG_OVERRUN);
    diag_update_timing(&d, 1.0f);
    diag_update_timing(&d, 3.0f);
    diag_update_peak(&d, 0.9f);
    diag_update_peak(&d, 1.2f);

    assert(d.xrun_count == 1UL);
    assert(d.overrun_count == 1UL);
    assert(d.clip_count == 1UL);
    assert(d.frames == 2UL);
    assert(d.avg_process_ms > 1.9f && d.avg_process_ms < 2.1f);
    assert(d.peak_abs > 1.1f);

    return 0;
}
