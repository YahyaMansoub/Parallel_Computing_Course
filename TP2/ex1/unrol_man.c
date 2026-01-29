#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

int main() {
    double *a = malloc(N * sizeof(double));
    double sum = 0.0;
    double start, end;

    for (int i = 0; i < N; i++)
        a[i] = 1.0;

    start = (double)clock() / CLOCKS_PER_SEC;

int i = 0;
for (; i + 4 <= N; i += 4)
    sum += a[i] + a[i+1] + a[i+2] + a[i+3];

for (; i < N; i++)
    sum += a[i];

end = (double)clock() / CLOCKS_PER_SEC;

    printf("Sum = %f, Time = %f ms\n", sum, (end - start) * 1000);

    free(a);
    return 0;
}
