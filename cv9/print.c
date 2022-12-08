#include "print.h"

void printDelimiter(bool full) {
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        if (full) printf("+---");
        else printf("+   ");
    }
    printf("+\n");
}

void printMatrix(char* matrix) {
    printDelimiter(true);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (!(i % 4)) printf("| %c   ", matrix[i]);
        else if (!((i + 1) % 16)) {
            printf("%c |\n", matrix[i]);
            printDelimiter(!((i + 1) % 64));
        }
        else if (!((i + 1) % 4)) printf("%c ", matrix[i]);
        else printf("%c   ", matrix[i]);
    }
}

void printPosCells(char* matrix, uintptr_t* posCells) {
    int count = 0;
    int countChars = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') continue;

        int x, y;
        getCoordinates(i, &x, &y);
        printf("%d:%d - ", x, y);
        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (cell->posibs[j]) {
                countChars++;
                printf("%c, ", 'a' + j);
            }
        }
        count++;
        printf("\n");
    }
    printf("%d, %d\n", count, countChars);
}