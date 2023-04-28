#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void fill(double **matrix, int rank, int size);
double calc(double u1, double u2, double u3, int i, int j);
void write_to_out(double** matrix, int n_m, int n_k);
void matrix_dump (FILE *out, double **matrix, int n_k, int n_m);
void line_dump (FILE *out, double *line, int n_m);
void fill_matrix(double **matrix, int n_k, int n_m);


void line_dump (FILE *out, double *line, int n_m)
{
  for (int m = 0; m <n_m; ++m)
  {
    //printf("%d", m);
    fprintf (out, "%.3f", line [m]);
    if (m + 1 != n_m)
      fprintf (out, ",");
  }
}

void matrix_dump (FILE *out, double **matrix, int n_k, int n_m)
{
  for (int k = 0; k < n_k; ++k)
  {
    line_dump (out, matrix [k], n_m);
    fprintf (out, "\n");
  }
}

void write_to_out(double** matrix, int n_m, int n_k) {
    FILE *fp = fopen("output.csv", "w+");
    matrix_dump(fp, matrix, n_k, n_m);
    fclose(fp);   
}

double calc(double u1, double u2, double u3, int i, int j) {                          //matrix[i + 1][j - 1] = u1
  return (2 * t_step * func((i + 0.5) * t_step, (j + 0.5) * x_step) /   //matrix[i][j - 1] = u2
             (1 + t_step / x_step)) -                                   //matrix[i][j] = u3
            (u1 * (1 - t_step / x_step) / (1 + t_step / x_step)) +
            (u2) + (u3 * (1 - t_step / x_step) / (1 + t_step / x_step));
}

void fill(double **matrix, int rank, int size) {
  int n_k = (int)(t_max / t_step) + 1;
  int n_m = (int)(x_max / x_step) + 1;
  int left_boarder = (n_m / size) * rank;
  int right_boarder = (n_m / size) * (rank + 1) - 1;
  MPI_Status status;

  if (rank == 0)
    left_boarder = 1;

  if (rank == size - 1)
    right_boarder = n_m - 1;


//Filling borders
  if (rank == 0) {
    for (int i = 0; i < n_k; i++) {
      matrix[i][0] = ksi(i * t_step);
    }
  }
//Filling borders
  for (int i = 0; i < n_k; i++) {
    for (int j = left_boarder; j <= right_boarder; j++) {
      if (i == 0) {
        matrix[i][j] = fi(j * x_step);
      }

      if ((rank != 0) && (j == left_boarder)) {
        MPI_Recv(&matrix[i][j - 1], 1, MPI_DOUBLE, rank - 1, 5, MPI_COMM_WORLD,
                 &status);
      }
      //Запоняем правую - верхнюю ячейку согласно схеме
      if (i < n_k - 1) {
        matrix[i + 1][j] = calc(matrix[i + 1][j - 1], matrix[i][j - 1], matrix[i][j], i, j);
      }
    }

    if (rank != (size - 1)) {
      MPI_Send(&matrix[i][right_boarder], 1, MPI_DOUBLE, rank + 1, 5, MPI_COMM_WORLD);
    }
  }
}

void fill_matrix(double **matrix, int n_k, int n_m) {
  int i, j;
  double frac = t_step / x_step;

  for (j = 0; j < n_m; j++) {
    matrix[0][j] = fi(j * x_step);
  }

  for (i = 0; i < n_k; i++) {
    matrix[i][0] = ksi(i * t_step);
  }

  for (i = 0; i < n_k - 1; i++) {
    for (j = 0; j < n_m; j++) {
      matrix[i + 1][j] =
          (2 * t_step * func((i + 0.5) * t_step, (j + 0.5) * x_step) /
           (1 + frac)) -
          (matrix[i + 1][j - 1] * (1 - frac) / (1 + frac)) +
          (matrix[i][j - 1]) + (matrix[i][j] * (1 - frac) / (1 + frac));
    }
  }
}
