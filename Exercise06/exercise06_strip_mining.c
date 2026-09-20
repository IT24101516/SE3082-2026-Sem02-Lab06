#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define STRIP_SIZE 64

int main()
{
    float *A = malloc(N * sizeof(float));
    float *B = malloc(N * sizeof(float));
    float *C = malloc(N * sizeof(float));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize arrays
    for (int i = 0; i < N; i++) {
        A[i] = 2.0f;
        B[i] = 3.0f;
    }

    int numberOfStrips =
        (N + STRIP_SIZE - 1) / STRIP_SIZE;

    double tstart = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int strip = 0; strip < numberOfStrips; strip++) {

        int start = strip * STRIP_SIZE;
        int end = start + STRIP_SIZE;

        if (end > N)
            end = N;

        #pragma omp simd
        for (int i = start; i < end; i++) {
            C[i] = A[i] * B[i];
        }
    }

    double tstop = omp_get_wtime();

    printf("C[0] = %.2f\n", C[0]);
    printf("C[N-1] = %.2f\n", C[N - 1]);

    printf(
        "Execution time = %.6f seconds\n",
        tstop - tstart
    );

    free(A);
    free(B);
    free(C);

    return 0;
}
