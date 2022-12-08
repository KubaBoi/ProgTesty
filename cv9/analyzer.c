#include "analyzer.h"

int analyze(char* matrix, uintptr_t* posCells) {
    int filled = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') {
            filled++;
            continue;  
        }

        if (cell->count == 0) return 1;
    }

    if (filled == MATRIX_SIZE) return 0;
    return 2;
}