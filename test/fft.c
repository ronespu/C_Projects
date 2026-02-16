#include <fftw3.h>
#include <stdio.h>

int main() {
    int N = 8;
    fftw_complex in[N], out[N];

    fftw_plan p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // llenar señal
    for(int i = 0; i < N; i++) {
        in[i][0] = i;   // parte real
        in[i][1] = 0.0; // parte imaginaria
    }

    fftw_execute(p);

    for(int i = 0; i < N; i++)
        printf("%f + %fi\n", out[i][0], out[i][1]);

    fftw_destroy_plan(p);
    return 0;
}