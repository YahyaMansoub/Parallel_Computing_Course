#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double t0 = MPI_Wtime();

    printf("Hello World\n");

    double t1 = MPI_Wtime();
    printf("Elapsed (this process): %f seconds\n", t1 - t0);

    MPI_Finalize();
    return 0;
} 