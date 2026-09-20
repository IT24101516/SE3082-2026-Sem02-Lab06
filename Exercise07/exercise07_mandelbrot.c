#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NPOINTS 2000
#define MAXITER 2000

struct complex {
    double real;
    double imag;
};

int main() {

    int i, j, iter;
    int numoutside = 0;

    double area, error, ztemp;
    double tstart, tstop, tcalc;

    struct complex z, c;

    /*
     * Start timing
     */

    tstart = omp_get_wtime();

    /*
     * Parallel Mandelbrot calculation
     */

    #pragma omp parallel private(i, j, iter, ztemp, z, c) reduction(+:numoutside)
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        /*
         * Distribute outer loop between threads
         */

        for (i = thread_id; i < NPOINTS; i += num_threads) {

            for (j = 0; j < NPOINTS; j++) {

                c.real = -2.0
                       + 2.5 * (double)(i) / (double)(NPOINTS)
                       + 1.0e-7;

                c.imag = 1.125
                       * (double)(j) / (double)(NPOINTS)
                       + 1.0e-7;

                z = c;

                for (iter = 0; iter < MAXITER; iter++) {

                    ztemp = (z.real * z.real)
                          - (z.imag * z.imag)
                          + c.real;

                    z.imag = z.real * z.imag * 2
                           + c.imag;

                    z.real = ztemp;

                    if ((z.real * z.real +
                         z.imag * z.imag) > 4.0e0) {

                        numoutside++;

                        break;
                    }
                }
            }
        }
    }

    /*
     * Stop timing
     */

    tstop = omp_get_wtime();

    tcalc = tstop - tstart;

    /*
     * Calculate area and error
     */

    area = 2.0 * 2.5 * 1.125
         * (double)(NPOINTS * NPOINTS - numoutside)
         / (double)(NPOINTS * NPOINTS);

    error = area / (double)NPOINTS;

    printf(
        "Area of Mandelbrot set = %12.8f +/- %12.8f\n",
        area,
        error
    );

    printf(
        "Execution time = %.6f seconds\n",
        tcalc
    );

    return 0;
}
