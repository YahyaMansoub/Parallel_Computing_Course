#include <stdio.h>
#include <omp.h>

static long num_steps = 100000;
double step;

int main() {
    int i;
    double pi, sum = 0.0;
    double start, end;

    step = 1.0 / (double) num_steps;

    start = omp_get_wtime();

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        double x;
        double local_sum = 0.0;

        /* manual work splitting */
        for (i = tid; i < num_steps; i += nthreads) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        /* safely accumulate */
        #pragma omp atomic
        sum += local_sum;
    }

    pi = step * sum;
    end = omp_get_wtime();

    printf("pi = %.15f\n", pi);
    printf("Time = %f seconds\n", end - start);

    return 0;
}
