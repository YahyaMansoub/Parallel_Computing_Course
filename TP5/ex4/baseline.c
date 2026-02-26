#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static double wtime_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}

void matrixVectorMult(const double* A, const double* b, double* x, int N) {
    for (int i = 0; i < N; ++i) {
        double sum = 0.0;
        for (int j = 0; j < N; ++j) {
            sum += A[i * N + j] * b[j];
        }
        x[i] = sum;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <matrix_size>\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    if (N <= 0) {
        printf("Matrix size must be positive.\n");
        return 1;
    }

    double* A = (double*)malloc((size_t)N * (size_t)N * sizeof(double));
    double* b = (double*)malloc((size_t)N * sizeof(double));
    double* x = (double*)malloc((size_t)N * sizeof(double));
    if (!A || !b || !x) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    srand48(42);

    // IMPORTANT: initialize all A to 0 (your snippet otherwise leaves garbage)
    for (int i = 0; i < N * N; ++i) A[i] = 0.0;

    // Fill A[0][:100] with random values
    int limit = (N < 100) ? N : 100;
    for (int j = 0; j < limit; ++j)
        A[0 * N + j] = drand48();

    // Copy A[0][:100] into A[1][100:200] if possible
    if (N > 1 && N > 100) {
        int copy_len = (N - 100 < 100) ? (N - 100) : 100;
        for (int j = 0; j < copy_len; ++j)
            A[1 * N + (100 + j)] = A[0 * N + j];
    }

    // Set diagonal
    for (int i = 0; i < N; ++i)
        A[i * N + i] = drand48();

    // Fill vector b
    for (int i = 0; i < N; ++i)
        b[i] = drand48();

    double t0 = wtime_sec();
    matrixVectorMult(A, b, x, N);
    double t1 = wtime_sec();

    printf("N=%d serial_time=%f s\n", N, t1 - t0);

    // optional: print one value to avoid “optimized away” worries
    printf("x[0]=%.6f\n", x[0]);

    free(A);
    free(b);
    free(x);
    return 0;
}