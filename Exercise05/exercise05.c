#include <stdio.h>
#include <omp.h>

long long fib_serial(int n)
{
    if (n < 2)
        return n;

    return fib_serial(n - 1) + fib_serial(n - 2);
}


long long fib_parallel(int n)
{
    if (n < 2)
        return n;

    // Avoid creating huge numbers of tiny OpenMP tasks
    if (n < 20)
        return fib_serial(n);

    long long i;
    long long j;

    #pragma omp task shared(i)
    i = fib_parallel(n - 1);

    #pragma omp task shared(j)
    j = fib_parallel(n - 2);

    #pragma omp taskwait

    return i + j;
}


int main()
{
    int n = 40;
    long long result;

    double tstart;
    double tstop;

    tstart = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = fib_parallel(n);
        }
    }

    tstop = omp_get_wtime();

    printf("Fib(%d) = %lld\n", n, result);
    printf("Time = %.6f seconds\n", tstop - tstart);

    return 0;
}
