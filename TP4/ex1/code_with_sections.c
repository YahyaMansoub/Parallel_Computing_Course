#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000

int main(void) {
    double *A = (double *)malloc((size_t)N * sizeof(double));
    if (!A) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialization (serial)
    srand(0);
    for (int i = 0; i < N; i++) {
        A[i] = (double)rand() / RAND_MAX;
    }

    // Shared results
    double sum = 0.0, mean = 0.0, stddev = 0.0, max = 0.0;
    int mean_ready = 0;   // flag for Section 3

    max = A[0];

    #pragma omp parallel shared(A, sum, mean, stddev, max, mean_ready)
    {
        #pragma omp sections
        {
            // Section 1: sum and mean
            #pragma omp section
            {
                double local_sum = 0.0;
                for (int i = 0; i < N; i++) {
                    local_sum += A[ i];
                }

                sum = local_sum;
                mean = sum / N;

                #pragma omp atomic write
                mean_ready = 1;

                #pragma omp flush(mean_ready, mean)
            }

            // Section 2: max
            #pragma omp section
            {
                double local_max = A[0];
                for (int i = 1; i < N; i++) {
                    if (A[i] > local_max) local_max = A[i];
                }
                max = local_max;
            }

            // Section 3: stddev (must use sum/mean from Section 1)
            #pragma omp section
            {
                int ready = 0;
                while (!ready) {
                    #pragma omp flush(mean_ready, mean)
                    ready = mean_ready;
                }

                double local_var = 0.0;
                for (int i = 0; i < N; i++) {
                    double d = A[i] - mean;
                    local_var += d * d;
                }
                stddev = sqrt(local_var / N);
            }
        } // implicit barrier here at end of sections
    } // end parallel

    printf("Sum     = %f\n", sum);
    printf("Max     = %f\n", max);
    printf("Std Dev = %f\n", stddev);

    free(A);
    return 0;
}
