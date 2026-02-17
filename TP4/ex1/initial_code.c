#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 1000000

int main() {

    double *A = malloc(N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    double sum = 0.0;
    double mean = 0.0;
    double stddev = 0.0;
    double max;

    // Initialization
    srand(0);
    for (int i = 0; i < N; i++)
        A[i] = (double)rand() / RAND_MAX;

    // Compute sum and max
    sum = 0.0;
    max = A[0];
    for (int i = 0; i < N; i++) {
        sum += A[i];
        if (A[i] > max)
            max = A[i];
    }

    // Compute mean
    mean = sum / N;

    // Compute standard deviation
    stddev = 0.0;
    for (int i = 0; i < N; i++)
        stddev += (A[i] - mean) * (A[i] - mean);

    stddev = sqrt(stddev / N);

    // Print results
    printf("Sum     = %f\n", sum);
    printf("Max     = %f\n", max);
    printf("Std Dev = %f\n", stddev);

    free(A);
    return 0;
}
