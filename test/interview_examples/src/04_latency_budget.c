#include <stdio.h>

static float frame_latency_ms(int frame, int sample_rate)
{
    return 1000.0f * (float)frame / (float)sample_rate;
}

int main(void)
{
    int sr = 48000;
    int frame_sizes[] = {64, 128, 256, 512, 1024};
    int i;

    for (i = 0; i < 5; ++i) {
        float one_way = frame_latency_ms(frame_sizes[i], sr);
        float round_trip = 2.0f * one_way;
        printf("frame=%4d -> one-way=%.2f ms, round-trip=%.2f ms\n",
               frame_sizes[i], one_way, round_trip);
    }

    printf("Interview point: lower frame = lower latency but higher CPU/interrupt overhead.\n");
    return 0;
}
