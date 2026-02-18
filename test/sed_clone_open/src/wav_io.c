#include "wav_io.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t read_u32_le(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8U) | ((uint32_t)p[2] << 16U) | ((uint32_t)p[3] << 24U);
}

static uint16_t read_u16_le(const uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8U);
}

int wav_read_mono16(const char *path, float **samples, size_t *count, wav_info_t *info)
{
    FILE *f;
    uint8_t hdr[44];
    uint32_t data_size;
    int16_t *pcm;
    size_t n;
    size_t i;

    if (!path || !samples || !count || !info) {
        return -1;
    }

    f = fopen(path, "rb");
    if (!f) {
        return -1;
    }

    if (fread(hdr, 1, sizeof(hdr), f) != sizeof(hdr)) {
        fclose(f);
        return -1;
    }

    if (memcmp(hdr, "RIFF", 4) != 0 || memcmp(hdr + 8, "WAVE", 4) != 0 || memcmp(hdr + 12, "fmt ", 4) != 0) {
        fclose(f);
        return -1;
    }

    info->channels = (int)read_u16_le(hdr + 22);
    info->sample_rate = (int)read_u32_le(hdr + 24);
    info->bits_per_sample = (int)read_u16_le(hdr + 34);

    if (info->channels != 1 || info->bits_per_sample != 16) {
        fclose(f);
        return -1;
    }

    if (memcmp(hdr + 36, "data", 4) != 0) {
        fclose(f);
        return -1;
    }

    data_size = read_u32_le(hdr + 40);
    n = data_size / 2U;
    pcm = (int16_t *)malloc(n * sizeof(int16_t));
    if (!pcm) {
        fclose(f);
        return -1;
    }

    if (fread(pcm, sizeof(int16_t), n, f) != n) {
        free(pcm);
        fclose(f);
        return -1;
    }

    *samples = (float *)malloc(n * sizeof(float));
    if (!*samples) {
        free(pcm);
        fclose(f);
        return -1;
    }

    for (i = 0U; i < n; ++i) {
        (*samples)[i] = (float)pcm[i] / 32768.0f;
    }

    free(pcm);
    fclose(f);
    *count = n;
    return 0;
}
