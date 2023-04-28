#include "func.c"
#include "stdlib.h"
#include "write.c"
#include <stdio.h>
#include <time.h>
//#include "mpi.h"

extern double func(double t, double x);
extern double fi(double x);
extern double ksi(double t);

extern double t_max;
extern double x_max;
extern double t_step;
extern double x_step;

int main(int argc, char **argv) {
  int i;
  int n_k = (int)(t_max / t_step) + 1;
  int n_m = (int)(x_max / x_step) + 1;
  double **matrix = (double **)calloc(n_k, sizeof(double *));

  for (i = 0; i < n_k; i++) {
    matrix[i] = (double *)calloc(n_m, sizeof(double));
  }

  //printf("n_k: %d n_m: %d\n", n_k, n_m);
  clock_t start,end;
  start=clock();
  //double start = MPI_Wtime();
  fill_matrix(matrix, n_k, n_m);
  //double end = MPI_Wtime();
  end=clock();
  printf("Time: %ld mc seconds\n", end - start);
  write_to_out(matrix, n_m, n_k);

  for (i = 0; i < n_k; i++) {
    free(matrix[i]);
  }

  free(matrix);

  return 0;
}
