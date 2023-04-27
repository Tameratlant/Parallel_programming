#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <stdlib.h>
#include "func.c"

extern double func  (double t, double x);
extern double fi    (double x);
extern double ksi   (double t);
int n_k, n_m;
extern double t_max;
extern double x_max;
extern double t_step;
extern double x_step;

double frac;

int get_high_border (int size, int rank, int low_border);
int get_low_border (int size, int rank);
void paral_algo (int size, int rank, double **matrix);
int main (int argc, char **argv)
{
	frac = t_step / x_step;
	n_k = (int) (t_max / t_step) + 1;
	n_m = (int) (x_max / x_step) + 1;
  int size, rank;
  MPI_Init (&argc, &argv);

  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	double **matrix = (double **)calloc (n_k, sizeof (*matrix));
  for (int i = 0; i <= n_k; ++i)
    matrix [i] = (double *)calloc (n_m, sizeof (double));
  double starttime = MPI_Wtime ();

    paral_algo (size, rank, matrix);
  double endtime = MPI_Wtime ();

  printf ("time: %f seconds\n", endtime - starttime);

  MPI_Finalize ();

  if (rank == 0) {
	for (int i = 0; i < n_k - 1; ++i) {
		for (int j = 0; j < n_m - 1; ++j) {
			printf("%.2lf ", matrix[i][j]);
		}
		printf ("\n");
	}

/*
	for (int i = 0; i < n_k; i++) {
                free(matrix[i]);
        }

	free(matrix);
*/
  }

  return 0;
}


int get_high_border (int size, int rank, int low_border)
{
  int part = (n_m) / size;
  int mod = (n_m) % size;

  return rank < mod ? low_border + part : low_border + part - 1;
}

/* Work-horse for parallel algorithm.  */
int get_low_border (int size, int rank)
{
  int part = (n_m) / size;
  int mod = (n_m) % size;

  return rank < mod ? rank * (part + 1) : rank * part + mod;
}

/* Parallel algorithm driver.  Let's count row-by-row (arrays marks
   evaluating-vector and numbers mean process's rank):

   TODO
*/
void paral_algo (int size, int rank, double **matrix)
{
  MPI_Status status;

  int low_border = get_low_border (size, rank);
  int high_border = get_high_border (size, rank, low_border);
  printf ("[rank %d]: calculating from %d to %d\n", rank, low_border,
		high_border);

  for (int k = 0; k <= n_k; ++k)
  {
    for (int m = low_border; m <= high_border; ++m)
    {
      /* Corner cases.  */
      if (k == 0)
	matrix [k] [m] = fi (m);
      else if (m == 0)
	matrix [k] [m] = ksi (k);
      else
      {
	/* If we're not the root proccess then we have to receive the previous
	   calculated data from previous proccess.  */
	if (rank != 0 && m == low_border)
	  MPI_Recv (&matrix [k] [m - 1], 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG,
	  	    MPI_COMM_WORLD, &status);

	/* Otherwise ours point's value depends on another three
	   points in rectangle's corners:

	   (m - 1, k) ---------- (m, k) <- calculate this one.
		|		   |
		|		   |
		|		   |
		|		   |
	   (m - 1, k - 1) --- (m, k - 1)
	   U^{k}_{m} = F_frac * f^{k - 1/2}_{m - 1/2} - U_frac * U^{k}_{m - 1}
			+ U^{k - 1}_{m - 1} + U_frac * U^{k - 1}_{m};
	*/
	/*matrix [k] [m] = F_frac * f (k, m) - U_frac * matrix [k] [m - 1]
			+ matrix [k - 1] [m - 1] + U_frac * matrix [k - 1] [m];*/

			matrix[k][m] = (2 * t_step * func((k-1 + 0.5) * t_step, (m + 0.5) * x_step) / (1 + frac)) -
				(matrix[k][m - 1] * (1 - frac) / (1 + frac)) + 
				(matrix[k-1][m - 1]) + (matrix[k-1][m] * (1 - frac) / (1 + frac));
      }
    }

    /* When we've counted current line let's send border to the next proccess.  */
    if (rank != (size - 1))
    	MPI_Send (&matrix [k] [high_border], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
  }
}
