#include "sed_extractor.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(void)
{
    const size_t n = 256;
    float frame[256];
    sed_extractor_t ext;
    sed_features_t f1;
    sed_features_t f2;
    size_t i;

    assert(sed_extractor_init(&ext, n, 16000.0f) == 0);

    for (i = 0U; i < n; ++i) {
        float t = (float)i / 16000.0f;
        frame[i] = sinf(2.0f * (float)M_PI * 1000.0f * t);
    }

    assert(sed_extractor_process(&ext, frame, &f1) == 0);
    assert(f1.rms > 0.4f && f1.rms < 0.8f);
    assert(f1.spectral_centroid_hz > 700.0f && f1.spectral_centroid_hz < 1300.0f);

    assert(sed_extractor_process(&ext, frame, &f2) == 0);
    assert(f2.spectral_flux < 0.02f);

    return 0;
}
