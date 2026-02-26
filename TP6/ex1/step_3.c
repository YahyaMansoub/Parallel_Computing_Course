
#include <mpi.h>
#include <stdio.h>

static void print_a(int a[4][5]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) printf("%d ", a[i][j]);
        printf("\n");
    }
}

static void print_at(int at[5][4]) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) printf("%d ", at[i][j]);
        printf("\n");
    }
}

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

        printf("Process 0 - Matrix a (before send):\n");
        print_a(a);
    }

    if (rank == 1) {
        int at[5][4] = {0};   // just allocate + init to 0

        printf("Process 1 - Matrix at (allocated, before recv):\n");
        print_at(at);
    }

    MPI_Finalize();
    return 0;
}