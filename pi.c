#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"


//mpicc pi.c -lm
//mpirun -n 4 ./a.out

/*

#define SIZE_PI 1000
int main(int argc, char **argv) {
    int rank;
    double Pi = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    for (int i = rank / MAX * SIZE_PI + 1; i < (rank + 1) / MAX * SIZE_PI; i++) {
        
        Pi +=  (double)1 / (i*i);
        printf("Thread № %d, PI = %f, i = %d\n", rank, Pi, i);
    }
    MPI_Finalize();
    Pi = sqrt(Pi * 6);
    printf("%f\n", Pi);
}
*/
#define MAX 100000
int main(int argc, char **argv) {
    int rank, size;
    double Pi = 0;
    double Pi_part = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("rank = %d\n", rank);
    Pi+=rank;
    //printf("%lf\n",Pi);

    if (rank != 0) {
        for (int i = 1 + (rank) / (size - 1) * MAX; i < 1 + (rank + 1) / (size - 1) * MAX; i++) {
            Pi_part += 1 / (i*i);
            //Pi_part += 1;
        }
        MPI_Send(&Pi_part, 1, MPI_FLOAT, 1, 5, MPI_COMM_WORLD);
    } else {
        
        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(&Pi_part, 1, MPI_FLOAT, 1, 5, MPI_COMM_WORLD, &status);
            Pi+=Pi_part;
        }
        
        printf("Pi = %lf\n", sqrt(Pi*6));
    }


}

/*
int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ibeg=rank / size SIZE_PI;
    iend=(rank+1) / size * SIZE_PI;
    for(i=ibeg + 1; i<iend; i++)
    printf ("process %d, %d^2=%d\n", rank, i,
    i*i);
    MPI_Finalize();
}
*/
