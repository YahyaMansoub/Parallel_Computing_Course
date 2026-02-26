
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int a[4][5];
        int v = 1;

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 5; ++j)
                a[i][j] = v++;

        printf("Process 0 - Matrix a:\n");
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 5; ++j) printf("%d ", a[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}