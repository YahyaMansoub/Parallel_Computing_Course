#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 1000000

static inline double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

// ========== DOUBLE TYPE ==========
#define BENCH_DOUBLE(U) \
static double sum_double_u##U(const double *a) { \
    double sum = 0.0; \
    int i = 0; \
    for (; i + U <= N; i += U) { \
        sum += a[i];
        
#define ADD_DOUBLE(idx) sum += a[i + idx];
#define END_DOUBLE \
    } \
    for (; i < N; i++) sum += a[i]; \
    return sum; \
}

// Generate unroll functions for double
BENCH_DOUBLE(1) END_DOUBLE
BENCH_DOUBLE(2) ADD_DOUBLE(1) END_DOUBLE
BENCH_DOUBLE(4) ADD_DOUBLE(1) ADD_DOUBLE(2) ADD_DOUBLE(3) END_DOUBLE
BENCH_DOUBLE(8) ADD_DOUBLE(1) ADD_DOUBLE(2) ADD_DOUBLE(3) ADD_DOUBLE(4) ADD_DOUBLE(5) ADD_DOUBLE(6) ADD_DOUBLE(7) END_DOUBLE
BENCH_DOUBLE(16) ADD_DOUBLE(1) ADD_DOUBLE(2) ADD_DOUBLE(3) ADD_DOUBLE(4) ADD_DOUBLE(5) ADD_DOUBLE(6) ADD_DOUBLE(7) ADD_DOUBLE(8) ADD_DOUBLE(9) ADD_DOUBLE(10) ADD_DOUBLE(11) ADD_DOUBLE(12) ADD_DOUBLE(13) ADD_DOUBLE(14) ADD_DOUBLE(15) END_DOUBLE
BENCH_DOUBLE(32) ADD_DOUBLE(1) ADD_DOUBLE(2) ADD_DOUBLE(3) ADD_DOUBLE(4) ADD_DOUBLE(5) ADD_DOUBLE(6) ADD_DOUBLE(7) ADD_DOUBLE(8) ADD_DOUBLE(9) ADD_DOUBLE(10) ADD_DOUBLE(11) ADD_DOUBLE(12) ADD_DOUBLE(13) ADD_DOUBLE(14) ADD_DOUBLE(15) ADD_DOUBLE(16) ADD_DOUBLE(17) ADD_DOUBLE(18) ADD_DOUBLE(19) ADD_DOUBLE(20) ADD_DOUBLE(21) ADD_DOUBLE(22) ADD_DOUBLE(23) ADD_DOUBLE(24) ADD_DOUBLE(25) ADD_DOUBLE(26) ADD_DOUBLE(27) ADD_DOUBLE(28) ADD_DOUBLE(29) ADD_DOUBLE(30) ADD_DOUBLE(31) END_DOUBLE

// ========== FLOAT TYPE ==========
#define BENCH_FLOAT(U) \
static float sum_float_u##U(const float *a) { \
    float sum = 0.0f; \
    int i = 0; \
    for (; i + U <= N; i += U) { \
        sum += a[i];

#define ADD_FLOAT(idx) sum += a[i + idx];
#define END_FLOAT \
    } \
    for (; i < N; i++) sum += a[i]; \
    return sum; \
}

BENCH_FLOAT(1) END_FLOAT
BENCH_FLOAT(2) ADD_FLOAT(1) END_FLOAT
BENCH_FLOAT(4) ADD_FLOAT(1) ADD_FLOAT(2) ADD_FLOAT(3) END_FLOAT
BENCH_FLOAT(8) ADD_FLOAT(1) ADD_FLOAT(2) ADD_FLOAT(3) ADD_FLOAT(4) ADD_FLOAT(5) ADD_FLOAT(6) ADD_FLOAT(7) END_FLOAT
BENCH_FLOAT(16) ADD_FLOAT(1) ADD_FLOAT(2) ADD_FLOAT(3) ADD_FLOAT(4) ADD_FLOAT(5) ADD_FLOAT(6) ADD_FLOAT(7) ADD_FLOAT(8) ADD_FLOAT(9) ADD_FLOAT(10) ADD_FLOAT(11) ADD_FLOAT(12) ADD_FLOAT(13) ADD_FLOAT(14) ADD_FLOAT(15) END_FLOAT
BENCH_FLOAT(32) ADD_FLOAT(1) ADD_FLOAT(2) ADD_FLOAT(3) ADD_FLOAT(4) ADD_FLOAT(5) ADD_FLOAT(6) ADD_FLOAT(7) ADD_FLOAT(8) ADD_FLOAT(9) ADD_FLOAT(10) ADD_FLOAT(11) ADD_FLOAT(12) ADD_FLOAT(13) ADD_FLOAT(14) ADD_FLOAT(15) ADD_FLOAT(16) ADD_FLOAT(17) ADD_FLOAT(18) ADD_FLOAT(19) ADD_FLOAT(20) ADD_FLOAT(21) ADD_FLOAT(22) ADD_FLOAT(23) ADD_FLOAT(24) ADD_FLOAT(25) ADD_FLOAT(26) ADD_FLOAT(27) ADD_FLOAT(28) ADD_FLOAT(29) ADD_FLOAT(30) ADD_FLOAT(31) END_FLOAT

// ========== INT TYPE ==========
#define BENCH_INT(U) \
static long long sum_int_u##U(const int *a) { \
    long long sum = 0; \
    int i = 0; \
    for (; i + U <= N; i += U) { \
        sum += a[i];

#define ADD_INT(idx) sum += a[i + idx];
#define END_INT \
    } \
    for (; i < N; i++) sum += a[i]; \
    return sum; \
}

BENCH_INT(1) END_INT
BENCH_INT(2) ADD_INT(1) END_INT
BENCH_INT(4) ADD_INT(1) ADD_INT(2) ADD_INT(3) END_INT
BENCH_INT(8) ADD_INT(1) ADD_INT(2) ADD_INT(3) ADD_INT(4) ADD_INT(5) ADD_INT(6) ADD_INT(7) END_INT
BENCH_INT(16) ADD_INT(1) ADD_INT(2) ADD_INT(3) ADD_INT(4) ADD_INT(5) ADD_INT(6) ADD_INT(7) ADD_INT(8) ADD_INT(9) ADD_INT(10) ADD_INT(11) ADD_INT(12) ADD_INT(13) ADD_INT(14) ADD_INT(15) END_INT
BENCH_INT(32) ADD_INT(1) ADD_INT(2) ADD_INT(3) ADD_INT(4) ADD_INT(5) ADD_INT(6) ADD_INT(7) ADD_INT(8) ADD_INT(9) ADD_INT(10) ADD_INT(11) ADD_INT(12) ADD_INT(13) ADD_INT(14) ADD_INT(15) ADD_INT(16) ADD_INT(17) ADD_INT(18) ADD_INT(19) ADD_INT(20) ADD_INT(21) ADD_INT(22) ADD_INT(23) ADD_INT(24) ADD_INT(25) ADD_INT(26) ADD_INT(27) ADD_INT(28) ADD_INT(29) ADD_INT(30) ADD_INT(31) END_INT

// ========== SHORT TYPE ==========
#define BENCH_SHORT(U) \
static long long sum_short_u##U(const short *a) { \
    long long sum = 0; \
    int i = 0; \
    for (; i + U <= N; i += U) { \
        sum += a[i];

#define ADD_SHORT(idx) sum += a[i + idx];
#define END_SHORT \
    } \
    for (; i < N; i++) sum += a[i]; \
    return sum; \
}

BENCH_SHORT(1) END_SHORT
BENCH_SHORT(2) ADD_SHORT(1) END_SHORT
BENCH_SHORT(4) ADD_SHORT(1) ADD_SHORT(2) ADD_SHORT(3) END_SHORT
BENCH_SHORT(8) ADD_SHORT(1) ADD_SHORT(2) ADD_SHORT(3) ADD_SHORT(4) ADD_SHORT(5) ADD_SHORT(6) ADD_SHORT(7) END_SHORT
BENCH_SHORT(16) ADD_SHORT(1) ADD_SHORT(2) ADD_SHORT(3) ADD_SHORT(4) ADD_SHORT(5) ADD_SHORT(6) ADD_SHORT(7) ADD_SHORT(8) ADD_SHORT(9) ADD_SHORT(10) ADD_SHORT(11) ADD_SHORT(12) ADD_SHORT(13) ADD_SHORT(14) ADD_SHORT(15) END_SHORT
BENCH_SHORT(32) ADD_SHORT(1) ADD_SHORT(2) ADD_SHORT(3) ADD_SHORT(4) ADD_SHORT(5) ADD_SHORT(6) ADD_SHORT(7) ADD_SHORT(8) ADD_SHORT(9) ADD_SHORT(10) ADD_SHORT(11) ADD_SHORT(12) ADD_SHORT(13) ADD_SHORT(14) ADD_SHORT(15) ADD_SHORT(16) ADD_SHORT(17) ADD_SHORT(18) ADD_SHORT(19) ADD_SHORT(20) ADD_SHORT(21) ADD_SHORT(22) ADD_SHORT(23) ADD_SHORT(24) ADD_SHORT(25) ADD_SHORT(26) ADD_SHORT(27) ADD_SHORT(28) ADD_SHORT(29) ADD_SHORT(30) ADD_SHORT(31) END_SHORT

static volatile double sink_d;
static volatile float sink_f;
static volatile long long sink_i;

void benchmark_double(void) {
    double *a = malloc(N * sizeof(double));
    if (!a) return;
    
    srand(1);
    for (int i = 0; i < N; i++) a[i] = (double)rand() / RAND_MAX;
    
    printf("type,U,ms,sum\n");
    
    double t0, t1, s;
    
    t0 = now_ms(); s = sum_double_u1(a); t1 = now_ms();
    printf("double,1,%.6f,%.6f\n", t1-t0, s); sink_d = s;
    
    t0 = now_ms(); s = sum_double_u2(a); t1 = now_ms();
    printf("double,2,%.6f,%.6f\n", t1-t0, s); sink_d = s;
    
    t0 = now_ms(); s = sum_double_u4(a); t1 = now_ms();
    printf("double,4,%.6f,%.6f\n", t1-t0, s); sink_d = s;
    
    t0 = now_ms(); s = sum_double_u8(a); t1 = now_ms();
    printf("double,8,%.6f,%.6f\n", t1-t0, s); sink_d = s;
    
    t0 = now_ms(); s = sum_double_u16(a); t1 = now_ms();
    printf("double,16,%.6f,%.6f\n", t1-t0, s); sink_d = s;
    
    t0 = now_ms(); s = sum_double_u32(a); t1 = now_ms();
    printf("double,32,%.6f,%.6f\n", t1-t0, s); sink_d = s;
    
    free(a);
}

void benchmark_float(void) {
    float *a = malloc(N * sizeof(float));
    if (!a) return;
    
    srand(1);
    for (int i = 0; i < N; i++) a[i] = (float)rand() / RAND_MAX;
    
    float sf;
    double t0, t1;
    
    t0 = now_ms(); sf = sum_float_u1(a); t1 = now_ms();
    printf("float,1,%.6f,%.6f\n", t1-t0, (double)sf); sink_f = sf;
    
    t0 = now_ms(); sf = sum_float_u2(a); t1 = now_ms();
    printf("float,2,%.6f,%.6f\n", t1-t0, (double)sf); sink_f = sf;
    
    t0 = now_ms(); sf = sum_float_u4(a); t1 = now_ms();
    printf("float,4,%.6f,%.6f\n", t1-t0, (double)sf); sink_f = sf;
    
    t0 = now_ms(); sf = sum_float_u8(a); t1 = now_ms();
    printf("float,8,%.6f,%.6f\n", t1-t0, (double)sf); sink_f = sf;
    
    t0 = now_ms(); sf = sum_float_u16(a); t1 = now_ms();
    printf("float,16,%.6f,%.6f\n", t1-t0, (double)sf); sink_f = sf;
    
    t0 = now_ms(); sf = sum_float_u32(a); t1 = now_ms();
    printf("float,32,%.6f,%.6f\n", t1-t0, (double)sf); sink_f = sf;
    
    free(a);
}

void benchmark_int(void) {
    int *a = malloc(N * sizeof(int));
    if (!a) return;
    
    srand(1);
    for (int i = 0; i < N; i++) a[i] = rand() % 100;
    
    long long si;
    double t0, t1;
    
    t0 = now_ms(); si = sum_int_u1(a); t1 = now_ms();
    printf("int,1,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_int_u2(a); t1 = now_ms();
    printf("int,2,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_int_u4(a); t1 = now_ms();
    printf("int,4,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_int_u8(a); t1 = now_ms();
    printf("int,8,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_int_u16(a); t1 = now_ms();
    printf("int,16,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_int_u32(a); t1 = now_ms();
    printf("int,32,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    free(a);
}

void benchmark_short(void) {
    short *a = malloc(N * sizeof(short));
    if (!a) return;
    
    srand(1);
    for (int i = 0; i < N; i++) a[i] = (short)(rand() % 100);
    
    long long si;
    double t0, t1;
    
    t0 = now_ms(); si = sum_short_u1(a); t1 = now_ms();
    printf("short,1,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_short_u2(a); t1 = now_ms();
    printf("short,2,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_short_u4(a); t1 = now_ms();
    printf("short,4,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_short_u8(a); t1 = now_ms();
    printf("short,8,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_short_u16(a); t1 = now_ms();
    printf("short,16,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    t0 = now_ms(); si = sum_short_u32(a); t1 = now_ms();
    printf("short,32,%.6f,%lld\n", t1-t0, si); sink_i = si;
    
    free(a);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <double|float|int|short>\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "double") == 0) {
        benchmark_double();
    } else if (strcmp(argv[1], "float") == 0) {
        benchmark_float();
    } else if (strcmp(argv[1], "int") == 0) {
        benchmark_int();
    } else if (strcmp(argv[1], "short") == 0) {
        benchmark_short();
    } else {
        fprintf(stderr, "Unknown type: %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}
