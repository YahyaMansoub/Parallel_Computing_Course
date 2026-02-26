#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double t0 = MPI_Wtime();

    if (rank == 0) {
        printf("Hello World (only rank 0). Total processes = %d\n", size);
    }

    double t1 = MPI_Wtime();
    
    printf("Rank %d elapsed: %f seconds\n", rank, t1 - t0);

    
    return 0;
} 