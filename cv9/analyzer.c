#include "analyzer.h"

/*
finds cell with greatest count of possibilities
and returns its index in matrix
*/
int findMax(char* matrix, uintptr_t* posCells) {
    int max = 0;
    int maxIndex = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (cell->count > max) {
            max = cell->count;
            maxIndex = i;
        }
    }
    return maxIndex;
}

/*
finds cell with lowest count of possibilities
and returns its index in matrix
*/
int findMin(char* matrix, uintptr_t* posCells) {
    int min = MATRIX_LENGTH;
    int minIndex = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (cell->count > 0 && cell->count < min) {
            min = cell->count;
            minIndex = i;
        }
    }
    return minIndex;
}

int analyze(char* matrix, uintptr_t* posCells) {
    int filled = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') {
            filled++;
            continue;  
        }

        if (cell->count == 0) return 0;
    }

    if (filled == MATRIX_SIZE) return 1;
    //return 2 + findMax(matrix, posCells);
    return 2 + findMin(matrix, posCells);
}

double percentage(char* matrix) {
    int counter = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') counter++;
    }
    return (double)counter / (MATRIX_SIZE) * 100;
}