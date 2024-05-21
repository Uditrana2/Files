#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000 
//generate 1000 X 1000 matrix of random element and then perform multiplication operation

void generate_matrix(double **matrix) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matrix[i][j] = rand() % 100; 
        }
    }
}


void print_matrix(double **matrix) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}


void matrix_multiply_seq(double **A, double **B, double **C) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void matrix_multiply_par(double **A, double **B, double **C) {
    int i, j, k;
    #pragma omp parallel for private(i, j, k) shared(A, B, C)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    double **A, **B, **C;
    int i;

   
    A = (double **)malloc(N * sizeof(double *));
    B = (double **)malloc(N * sizeof(double *));
    C = (double **)malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
        B[i] = (double *)malloc(N * sizeof(double));
        C[i] = (double *)malloc(N * sizeof(double));
    }

    
    generate_matrix(A);
    generate_matrix(B);

    printf("Matrix A:\n");
    print_matrix(A);
    printf("\nMatrix B:\n");
    print_matrix(B);

    
    double start_time = omp_get_wtime();
    matrix_multiply_seq(A, B, C);
    double seq_time = omp_get_wtime() - start_time;

    
    start_time = omp_get_wtime();
    matrix_multiply_par(A, B, C);
    double par_time = omp_get_wtime() - start_time;

    printf("\nSequential execution time: %f seconds\n", seq_time);
    printf("Parallel execution time: %f seconds\n", par_time);

    
    for (i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
