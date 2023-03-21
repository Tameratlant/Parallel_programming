#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

#define MAX 100000
int main(int argc, char **argv) {
	int size, rank;
   	double pi = 0.0;
	int last = 1000000;
	int l;
	int r;
    double Pi_part = 0;
    	MPI_Status status;
    	MPI_Init(&argc, &argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);

        double buf;
    	if (rank != 0) {
            l = 1 + (double)(rank-1)/(size - 1) * MAX;
            r = 1 + (double)(rank)/(size - 1) * MAX;
            //printf("l = %d, r = %d\n", l, r);
            for (int i = l; i < r; i++) {
                if (i != 0) {
                    buf = (double)1 / i;
                    buf = buf / i;
                    Pi_part += buf;
                }
            }
            printf("%d thread's part = %lf\n", rank, Pi_part);
		    MPI_Send(&Pi_part, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
    	}

    	if (rank == 0) {
            double recv;
	    	for (int i = 1; i < size; i++) {
                MPI_Recv(&recv, 1, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &status);
                pi += recv;
	    	}
		    pi = sqrt(6 * pi);
	    	printf("pi: %f\n", pi);
    	}
	
	MPI_Finalize();
	return 0;
}