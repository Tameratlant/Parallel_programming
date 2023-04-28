#include "func.c"
#include "write.c"
extern double func(double t, double x);
extern double fi(double x);
extern double ksi(double t);

extern double t_max;
extern double x_max;
extern double t_step;
extern double x_step;
/*
double calc(double u1, double u2, double u3, int i, int j);
void fill(double **matrix, int rank, int size);
*/
int main(int argc, char **argv) {
  int i, j;
  int rank, size;
  int n_k = (int)(t_max / t_step) + 1;
  int n_m = (int)(x_max / x_step) + 1;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double **matrix = (double **)calloc(n_k, sizeof(double *));
  double **transposed = (double **)calloc(n_m, sizeof(double *));
  for (i = 0; i < n_k; i++) {
    matrix[i] = (double *)calloc(n_m, sizeof(double));
  }
  for (i = 0; i < n_m; i++) {
    transposed[i] = (double *)calloc(n_k, sizeof(double));
  }

  double start = MPI_Wtime();
  fill(matrix, rank, size);
  MPI_Barrier(MPI_COMM_WORLD);
  double end = MPI_Wtime();
  printf("rank: %d time: %f seconds\n", rank, end - start);




//Трансопнирование для вывода в файл
  int left_boarder = (n_m / size) * rank;
  int right_boarder = (n_m / size) * (rank + 1) - 1;

  if (rank == size - 1)
    right_boarder = n_m - 1;

  for (i = 0; i < n_k; i++) {
    for (j = left_boarder; j <= right_boarder; j++) {
      transposed[j][i] = matrix[i][j];
    }
  }

  if (rank != 0) {
    for (i = left_boarder; i <= right_boarder; i++) {
      MPI_Send(transposed[i], n_k, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
    }
  }

  if (rank == 0) {
    for (int k = 1; k < size; k++) {
      int left_boarder_k = (n_m / size) * k;
      int right_boarder_k = (n_m / size) * (k + 1) - 1;

      if (k == size - 1)
        right_boarder_k = n_m - 1;

      for (i = left_boarder_k; i <= right_boarder_k; i++) {
        MPI_Recv(transposed[i], n_k, MPI_DOUBLE, k, 5, MPI_COMM_WORLD, &status);
      }
    }
  }

  if (rank == 0)
    write_to_out(transposed, n_m, n_k);
  for (i = 0; i < n_k; i++) {
    free(matrix[i]);
  }

  for (i = 0; i < n_k; i++) {
    free(transposed[i]);
  }
  free(transposed);
  free(matrix);

  MPI_Finalize();

  return 0;
}
