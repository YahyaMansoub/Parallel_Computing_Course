// ex2_master_single.c
#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
  #include <omp.h>
#else
  #include <time.h>
  static double omp_get_wtime(void) { return (double)clock() / CLOCKS_PER_SEC; }
#endif

#ifndef N
#define N 8   // keep small if you really want to print; compile with -DN=1000 if needed
#endif

static void init_matrix(int n, double *A) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i*n + j] = (double)(i + j);
}

static void print_matrix(int n, const double *A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6.1f ", A[i*n + j]);
        printf("\n");
    }
}

int main(void) {
    double *A = (double*)malloc((size_t)N * (size_t)N * sizeof(double));
    if (!A) { perror("malloc"); return 1; }

    double t0 = omp_get_wtime();

#ifdef _OPENMP
    double sum = 0.0;

    #pragma omp parallel shared(A)
    {
        // 1) master initializes
        #pragma omp master
        {
            init_matrix(N, A);
        }

        // make sure init finished before print/sum
        #pragma omp barrier

        // 2) single prints
        #pragma omp single
        {
            print_matrix(N, A);
        }

        // 3) all threads compute sum in parallel
        #pragma omp for reduction(+:sum)
        for (int idx = 0; idx < N*N; idx++) {
            sum += A[idx];
        }
    }

    double t1 = omp_get_wtime();
    printf("Sum = %.6f\n", sum);
    printf("Time (OpenMP) = %.6f s\n", t1 - t0);

#else
    // Sequential version (no OpenMP)
    init_matrix(N, A);
    print_matrix(N, A);

    double sum = 0.0;
    for (int idx = 0; idx < N*N; idx++) sum += A[idx];

    double t1 = omp_get_wtime();
    printf("Sum = %.6f\n", sum);
    printf("Time (Serial) = %.6f s\n", t1 - t0);
#endif

    free(A);
    return 0;
}
