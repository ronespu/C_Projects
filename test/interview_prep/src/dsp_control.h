#ifndef DSP_CONTROL_H
#define DSP_CONTROL_H

typedef struct {
    float gain;
    int bypass;
} dsp_state_t;

typedef enum {
    CMD_SET_GAIN = 0,
    CMD_SET_BYPASS = 1,
    CMD_RESET = 2
} dsp_cmd_t;

void dsp_state_init(dsp_state_t *state);
int dsp_apply_command(dsp_state_t *state, dsp_cmd_t cmd, float value);
float dsp_process_sample(const dsp_state_t *state, float in_sample);

#endif
