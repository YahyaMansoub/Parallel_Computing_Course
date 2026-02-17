
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifndef N
#define N 2000000
#endif

static double task_light(int N) {
    double x = 0.0;
    for (int i = 0; i < N; i++) x += sin(i * 0.001);
    return x;
}

static double task_moderate(int N) {
    double x = 0.0;
    for (int i = 0; i < 5*N; i++) x += sqrt(i * 0.5) * cos(i * 0.001);
    return x;
}

static double task_heavy(int N) {
    double x = 0.0;
    for (int i = 0; i < 20*N; i++) x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    return x;
}

static double wtime(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}

int main(void) {
    double t0 = wtime();

    double a = task_light(N);
    double b = task_moderate(N);
    double c = task_heavy(N);

    double t1 = wtime();

    printf("Results: A=%f B=%f C=%f (ignore values)\n", a, b, c);
    printf("Time (serial) = %f s\n", t1 - t0);
    return 0;
}
