#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define ITERATIONS_NUM 1000000

int main(int argc, char **argv) {
    int size, rank;
    int foo;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

  	if (rank == 0) {
		clock_t start_t = clock();

		for (int i = 0; i < ITERATIONS_NUM; i++) {
			MPI_Send(&foo, 1, MPI_INT, 1, 5, MPI_COMM_WORLD);
			MPI_Recv(&foo, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
		}

		clock_t end_t = clock();

		double time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
		
		printf("%lf u - время в одну сторону\n", time * 1000000 / (2.0 * ITERATIONS_NUM));
        printf("Total time: %lf\n", time);
	} else {
        for (int i = 0; i < ITERATIONS_NUM; i++) {
            MPI_Recv(&foo, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
		    MPI_Send(&foo, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}