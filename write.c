#include <stdio.h>
#include <stdlib.h>



void line_dump (FILE *out, double *line, int n_m)
{
  for (int m = 0; m <n_m; ++m)
  {
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

void write_to_csv(double** matrix, int n_m, int n_k) {
    FILE *fp = fopen("output.csv", "w+");
    matrix_dump(fp, matrix, n_k, n_m);
    fclose(fp);   
}
