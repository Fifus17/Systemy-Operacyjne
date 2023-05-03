#include "zad1.h"
#include <stdlib.h>

void memory(unsigned int rows, unsigned int cols)
{
	// tu miales zaalokowac macierz, check i pozinej free
    double **matrix;

    matrix = (double **) calloc(rows, sizeof(double*));

    for(int i=0; i<rows; i++){
        matrix[i] = (double *) calloc(cols, sizeof(double));
    } 

    check_matrix(matrix, rows, cols);
    
    for(int i=0; i<rows; i++){
        free(matrix[i]);
    }

    free(matrix);
}

int main(int arc, char **argv)
{
    srand(42);
    unsigned int cols = rand() % 20;
    unsigned int rows = rand() % 20;
    memory(cols, rows);
    check_memory(cols, rows);
}
