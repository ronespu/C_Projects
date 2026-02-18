#include "dsp_control.h"

void dsp_state_init(dsp_state_t *state)
{
    if (!state) {
        return;
    }
    state->gain = 1.0f;
    state->bypass = 0;
}

int dsp_apply_command(dsp_state_t *state, dsp_cmd_t cmd, float value)
{
    if (!state) {
        return -1;
    }

    switch (cmd) {
    case CMD_SET_GAIN:
        if ((value < 0.0f) || (value > 8.0f)) {
            return -1;
        }
        state->gain = value;
        return 0;
    case CMD_SET_BYPASS:
        state->bypass = (value >= 0.5f) ? 1 : 0;
        return 0;
    case CMD_RESET:
        dsp_state_init(state);
        return 0;
    default:
        return -1;
    }
}

float dsp_process_sample(const dsp_state_t *state, float in_sample)
{
    if (!state || state->bypass) {
        return in_sample;
    }
    return in_sample * state->gain;
}
