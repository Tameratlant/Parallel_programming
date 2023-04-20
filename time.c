#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define ITERATIONS_NUM 1000000

int pow(int a, int b);

int main(int argc, char **argv) {
    int size, rank;
    int foo[1000000];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int poww = 6;
    for (int cur_pow = 0; cur_pow < poww; cur_pow++) {
          	if (rank == 0) {
		        clock_t start_t = 1000 * MPI_Wtime ();

                for (int i = 0; i < ITERATIONS_NUM; i++) {
                    MPI_Send(foo, pow(10, cur_pow), MPI_INT, 1, 5, MPI_COMM_WORLD);
                    MPI_Recv(foo, pow(10, cur_pow), MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
                }

                clock_t end_t =  1000 * MPI_Wtime ();

                double time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                
                printf("Fow element's number = 10^%d: %lf m - время в одну сторону\n", cur_pow, time * 1000000 / (2.0 * ITERATIONS_NUM));
               // printf("%lf ", time);
	    } else {
            for (int i = 0; i < ITERATIONS_NUM; i++) {
                MPI_Recv(foo, pow(10, cur_pow), MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
                MPI_Send(foo, pow(10, cur_pow), MPI_INT, 0, 5, MPI_COMM_WORLD);
            }
        }
    }


    MPI_Finalize();
    return 0;
}

int pow(int a, int b) {
    int result = 1;
    for (int i = 0; i < b; ++i) {
        result = result * a;
    }
}
