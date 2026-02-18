#include <assert.h>
#include <math.h>

#include "sed_open.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(void)
{
    sed_open_t st;
    int8_quant_params_t q = {0.9999854f, 0};
    sed_frame_features_t feat;
    int ready = 0;
    int sr = 16000;
    int n = 8192;
    int i;

    assert(sed_open_init(&st, sr) == 0);

    for (i = 0; i < n; ++i) {
        float t = (float)i / (float)sr;
        float s = 0.8f * sinf(2.0f * (float)M_PI * 1000.0f * t);
        assert(sed_open_push(&st, s, &q, &feat, &ready) == 0);
    }

    assert(ready == 1);
    assert(feat.rms > 0.1f);
    assert(feat.centroid_hz > 500.0f && feat.centroid_hz < 1500.0f);
    assert(feat.logmel[0] <= 0.0f);

    sed_open_deinit(&st);
    return 0;
}
