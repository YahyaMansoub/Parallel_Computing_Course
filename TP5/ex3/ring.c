#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size, x;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int prev = (rank - 1 + size) % size;
    int next = (rank + 1) % size;

    if (rank == 0) {
        scanf("%d", &x);
        MPI_Send(&x, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&x, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Process %d got %d\n", rank, x);
    } else {
        MPI_Recv(&x, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        x += rank;
        printf("Process %d got %d\n", rank, x);
        MPI_Send(&x, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}  