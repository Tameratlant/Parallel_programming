#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MATRIX_SIZE 8
#define QUADR_SIZE 4

int main(int argc, char **argv) {
	int quadrant[QUADR_SIZE][QUADR_SIZE];
	int tquadrant[QUADR_SIZE][QUADR_SIZE]; 
	int size, rank;

    int** matrix;
	int** tmatrix;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);   

    int row, column;
	if (rank == 0) {

        //Allocation memory
		matrix = (int **)calloc(MATRIX_SIZE, sizeof(int *));
        for (int i = 0; i < MATRIX_SIZE; i++) {
            matrix[i] = (int *)calloc(MATRIX_SIZE, sizeof(int));
        }

		tmatrix = (int **)calloc(MATRIX_SIZE, sizeof(int *));
		for (int i = 0; i < MATRIX_SIZE; i++) {
			tmatrix[i] = (int *)calloc(MATRIX_SIZE, sizeof(int));
		}
        //


		//Заполнение последовательными числами
		for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                matrix[j][k] = j * MATRIX_SIZE + k;
            }
        }


        //Разделение на квадратны и отправа на обработку
		for (int i = 1; i < size; i++) {
			row = i / (MATRIX_SIZE / QUADR_SIZE);
			column = i % (MATRIX_SIZE / QUADR_SIZE);

			for (int j = 0; j < QUADR_SIZE; j++) {
				for (int k = 0; k < QUADR_SIZE; k++) {
					quadrant[j][k] = matrix[row * QUADR_SIZE + j][column * QUADR_SIZE + k];
				}
			}
			MPI_Send(quadrant, QUADR_SIZE*QUADR_SIZE, MPI_INT, i, 5, MPI_COMM_WORLD);
		}


		for (int j = 0; j < QUADR_SIZE; j++) {
            for (int k = 0; k < QUADR_SIZE; k++) {
                quadrant[j][k] = matrix[j][k];
            }
		}
        for (int j = 0; j < QUADR_SIZE; j++)
            for (int k = 0; k < QUADR_SIZE; k++)
                tquadrant[j][k] = quadrant[k][j];						//можно оптимизировать по памяти, не создавая лишнюю матрицу
	}


	if (rank != 0) {
		MPI_Recv(&quadrant, QUADR_SIZE*QUADR_SIZE, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
		for (int j = 0; j < QUADR_SIZE; j++)
			for (int k = 0; k < QUADR_SIZE; k++)
				tquadrant[j][k] = quadrant[k][j];						//можно оптимизировать по памяти, не создавая лишнюю матрицу

		MPI_Send(tquadrant, QUADR_SIZE*QUADR_SIZE, MPI_INT, 0, 5, MPI_COMM_WORLD);
	}

    if (rank == 0) {
		//Заполняем итогову матрицу
		///////////////////////////////////////////////////////////

		//Верхний квадрант
		for (int j = 0; j < QUADR_SIZE; j++) {
            for (int k = 0; k < QUADR_SIZE; k++) {
                tmatrix[j][k] = tquadrant[j][k];
            }
        }
		
		//Остальные
        for (int i = 1; i < size; i++) {
			MPI_Recv(tquadrant, QUADR_SIZE*QUADR_SIZE, MPI_INT, i, 5, MPI_COMM_WORLD, &status);
            row = i % (MATRIX_SIZE / QUADR_SIZE);
            column = i / (MATRIX_SIZE / QUADR_SIZE);
            for (int j = 0; j < QUADR_SIZE; j++) {
                for (int k = 0; k < QUADR_SIZE; k++) {
                    tmatrix[row * QUADR_SIZE  + j][column * QUADR_SIZE + k] = tquadrant[j][k];
                }
			}
        }


		
		printf("Result matrix:\n");
		for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                printf ("%6d", tmatrix[j][k]);
			}
            printf("\n");
		}


        //free
		for (int i = 0; i < MATRIX_SIZE; i++) {
            free(tmatrix[i]);
		}

		for (int i = 0; i < MATRIX_SIZE; i++) {
            free(matrix[i]);
        }
		free(tmatrix);
		free(matrix);
	}

	MPI_Finalize();

	return 0;
}