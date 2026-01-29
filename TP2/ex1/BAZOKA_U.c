#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

static inline double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

static volatile double sink; // prevents optimizing away results

static double sum_u1(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 1 <= N; i += 1) { sum += a[i]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u2(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 2 <= N; i += 2) { sum += a[i] + a[i+1]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u3(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 3 <= N; i += 3) { sum += a[i] + a[i+1] + a[i+2]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u4(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 4 <= N; i += 4) { sum += a[i] + a[i+1] + a[i+2] + a[i+3]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u5(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 5 <= N; i += 5) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u6(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 6 <= N; i += 6) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u7(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 7 <= N; i += 7) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u8(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 8 <= N; i += 8) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u9(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 9 <= N; i += 9) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u10(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 10 <= N; i += 10) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u11(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 11 <= N; i += 11) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u12(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 12 <= N; i += 12) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u13(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 13 <= N; i += 13) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u14(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 14 <= N; i += 14) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u15(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 15 <= N; i += 15) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u16(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 16 <= N; i += 16) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u17(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 17 <= N; i += 17) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u18(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 18 <= N; i += 18) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u19(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 19 <= N; i += 19) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u20(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 20 <= N; i += 20) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u21(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 21 <= N; i += 21) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u22(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 22 <= N; i += 22) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u23(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 23 <= N; i += 23) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u24(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 24 <= N; i += 24) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u25(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 25 <= N; i += 25) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u26(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 26 <= N; i += 26) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u27(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 27 <= N; i += 27) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u28(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 28 <= N; i += 28) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u29(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 29 <= N; i += 29) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u30(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 30 <= N; i += 30) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28] + a[i+29]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u31(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 31 <= N; i += 31) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28] + a[i+29] + a[i+30]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

static double sum_u32(const double *a) {
    double sum = 0.0;
    int i = 0;
    for (; i + 32 <= N; i += 32) { sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28] + a[i+29] + a[i+30] + a[i+31]; }
    for (; i < N; i++) sum += a[i];
    return sum;
}

typedef double (*sum_fn)(const double*);

int main(void) {
    double *a = (double*)malloc((size_t)N * sizeof(double));
    if (!a) return 1;

    srand(1); // fixed seed => same data every run
    for (int i = 0; i < N; i++) a[i] = (double)rand() / (double)RAND_MAX;

    sum_fn f[33] = {
        NULL,
        sum_u1, sum_u2, sum_u3, sum_u4, sum_u5, sum_u6, sum_u7, sum_u8,
        sum_u9, sum_u10, sum_u11, sum_u12, sum_u13, sum_u14, sum_u15, sum_u16,
        sum_u17, sum_u18, sum_u19, sum_u20, sum_u21, sum_u22, sum_u23, sum_u24,
        sum_u25, sum_u26, sum_u27, sum_u28, sum_u29, sum_u30, sum_u31, sum_u32
    };

    printf("U,ms,sum\n");
    for (int u = 1; u <= 32; u++) {
        double t0 = now_ms();
        double s  = f[u](a);
        double t1 = now_ms();
        sink = s;
        printf("%d,%.6f,%.6f\n", u, (t1 - t0), s);
    }

    free(a);
    return 0;
}

