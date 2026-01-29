#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

#ifndef U
#define U 1
#endif




static inline double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}


int main(void) {
    double *a = (double*)malloc(N * sizeof(double));
    if (!a) return 1;

    // Fill with runtime values so -O2 can't "precompute" the sum
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++) a[i] = (double)rand() / (double)RAND_MAX;

    double sum = 0.0;

    double start = now_ms();

    int i = 0;
    for (; i + U <= N; i += U) {
#if U >= 1
        sum += a[i];
#endif
#if U >= 2
        sum += a[i+1];
#endif
#if U >= 3
        sum += a[i+2];
#endif
#if U >= 4
        sum += a[i+3];
#endif
#if U >= 5
        sum += a[i+4];
#endif
#if U >= 6
        sum += a[i+5];
#endif
#if U >= 7
        sum += a[i+6];
#endif
#if U >= 8
        sum += a[i+7];
#endif
#if U >= 9
        sum += a[i+8];
#endif
#if U >= 10
        sum += a[i+9];
#endif
#if U >= 11
        sum += a[i+10];
#endif
#if U >= 12
        sum += a[i+11];
#endif
#if U >= 13
        sum += a[i+12];
#endif
#if U >= 14
        sum += a[i+13];
#endif
#if U >= 15
        sum += a[i+14];
#endif
#if U >= 16
        sum += a[i+15];
#endif
#if U >= 17
        sum += a[i+16];
#endif
#if U >= 18
        sum += a[i+17];
#endif
#if U >= 19
        sum += a[i+18];
#endif
#if U >= 20
        sum += a[i+19];
#endif
#if U >= 21
        sum += a[i+20];
#endif
#if U >= 22
        sum += a[i+21];
#endif
#if U >= 23
        sum += a[i+22];
#endif
#if U >= 24
        sum += a[i+23];
#endif
#if U >= 25
        sum += a[i+24];
#endif
#if U >= 26
        sum += a[i+25];
#endif
#if U >= 27
        sum += a[i+26];
#endif
#if U >= 28
        sum += a[i+27];
#endif
#if U >= 29
        sum += a[i+28];
#endif
#if U >= 30
        sum += a[i+29];
#endif
#if U >= 31
        sum += a[i+30];
#endif
#if U >= 32
        sum += a[i+31];
#endif
    }

    // remainder
    for (; i < N; i++) sum += a[i];

    double end = now_ms();

    printf("%d,%.6f\n", U, (end - start));   // U,ms

    free(a);
    return 0;
}

