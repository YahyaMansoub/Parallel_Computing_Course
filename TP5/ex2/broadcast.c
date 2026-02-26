#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size, x = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    while (1) {
        if (rank == 0) {
            if (scanf("%d", &x) != 1) x = -1;  
        }

        MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (x < 0) break;

        printf("Process %d got %d\n", rank, x);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}