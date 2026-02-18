#include <stdio.h>

typedef enum {
    CMD_SET_GAIN,
    CMD_SET_BYPASS
} cmd_type_t;

typedef struct {
    cmd_type_t type;
    float value;
} command_t;

typedef struct {
    float gain;
    int bypass;
} dsp_state_t;

static void apply_cmd(dsp_state_t *s, command_t c)
{
    if (c.type == CMD_SET_GAIN && c.value >= 0.0f && c.value <= 4.0f) {
        s->gain = c.value;
    } else if (c.type == CMD_SET_BYPASS) {
        s->bypass = (c.value >= 0.5f) ? 1 : 0;
    }
}

static float process_sample(const dsp_state_t *s, float in)
{
    if (s->bypass) {
        return in;
    }
    return in * s->gain;
}

int main(void)
{
    dsp_state_t st = {1.0f, 0};
    command_t q[] = {
        {CMD_SET_GAIN, 1.5f},
        {CMD_SET_BYPASS, 0.0f},
        {CMD_SET_GAIN, 0.7f},
        {CMD_SET_BYPASS, 1.0f}
    };
    float input[] = {0.1f, 0.2f, -0.2f, 0.4f};
    int i;

    for (i = 0; i < 4; ++i) {
        apply_cmd(&st, q[i]);
        printf("cmd[%d]: gain=%.2f bypass=%d\n", i, st.gain, st.bypass);
        printf("  sample in=%.3f out=%.3f\n", input[i], process_sample(&st, input[i]));
    }

    printf("Interview point: command/control plane separado del audio data path.\n");
    return 0;
}
