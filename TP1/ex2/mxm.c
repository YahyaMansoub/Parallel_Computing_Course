#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

static void *xaligned_malloc(size_t bytes, size_t alignment) {
    void *p = NULL;
    if (posix_memalign(&p, alignment, bytes) != 0) return NULL;
    return p;
}

static void init_mat(double *A, double *B, double *C, int n) {
    for (int i = 0; i < n*n; i++) {
        A[i] = (double)((i % 97) - 48) * 0.01;
        B[i] = (double)((i % 89) - 44) * 0.01;
        C[i] = 0.0;
    }
}

static double checksum(const double *C, int n) {
    double s = 0.0;
    for (int i = 0; i < n*n; i += (n*n/1024 + 1)) s += C[i];
    return s;
}

/* Standard order: i-j-k (good for A row, bad for B column in row-major) */
static void mxm_ijk(const double *A, const double *B, double *C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double cij = C[(size_t)i*n + j];          // load once
            for (int k = 0; k < n; k++) {
                cij += A[(size_t)i*n + k] * B[(size_t)k*n + j];
            }
            C[(size_t)i*n + j] = cij;                 // store once
        }
    }
}

/* Reordered: i-k-j (stream B row + C row contiguously; reuse A scalar) */
static void mxm_ikj(const double *A, const double *B, double *C, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = A[(size_t)i*n + k];          // load once per (i,k)
            const double *Bk = &B[(size_t)k*n];
            double *Ci = &C[(size_t)i*n];
            for (int j = 0; j < n; j++) {
                Ci[j] += aik * Bk[j];
            }
        }
    }
}

int main(int argc, char **argv) {
    int n = (argc >= 2) ? atoi(argv[1]) : 1024;
    int reps = (argc >= 3) ? atoi(argv[2]) : 3;

    size_t bytes = (size_t)n * (size_t)n * sizeof(double);
    double *A = xaligned_malloc(bytes, 64);
    double *B = xaligned_malloc(bytes, 64);
    double *C = xaligned_malloc(bytes, 64);
    if (!A || !B || !C) { fprintf(stderr, "alloc failed\n"); return 1; }

    printf("kernel,n,reps,time_ms,gflops,bandwidth_GBps,check\n");

    // ---- IJK ----
    init_mat(A, B, C, n);
    double t0 = now_sec();
    for (int r = 0; r < reps; r++) mxm_ijk(A, B, C, n);
    double t1 = now_sec();
    double sec = t1 - t0;

    double flops = (double)reps * 2.0 * (double)n * (double)n * (double)n;
    double gflops = flops / sec / 1e9;

    // simple access-model bytes (not “real DRAM bytes”, but good for comparison)
    // IJK: A load n^3, B load n^3, C load+store n^2  -> (16*n^3 + 16*n^2) bytes for double
    double bytes_ijk = (double)reps * (16.0 * (double)n*n*n + 16.0 * (double)n*n);
    double bw_ijk = bytes_ijk / sec / 1e9;

    printf("ijk,%d,%d,%.3f,%.3f,%.3f,%.6e\n",
           n, reps, sec*1000.0, gflops, bw_ijk, checksum(C, n));

    // ---- IKJ ----
    init_mat(A, B, C, n);
    t0 = now_sec();
    for (int r = 0; r < reps; r++) mxm_ikj(A, B, C, n);
    t1 = now_sec();
    sec = t1 - t0;

    gflops = flops / sec / 1e9;

    // IKJ: A load n^2, B load n^3, C load+store n^3 -> (24*n^3 + 8*n^2) bytes for double
    double bytes_ikj = (double)reps * (24.0 * (double)n*n*n + 8.0 * (double)n*n);
    double bw_ikj = bytes_ikj / sec / 1e9;

    printf("ikj,%d,%d,%.3f,%.3f,%.3f,%.6e\n",
           n, reps, sec*1000.0, gflops, bw_ikj, checksum(C, n));

    free(A); free(B); free(C);
    return 0;
}
