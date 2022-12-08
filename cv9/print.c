#include "print.h"

void print(const char* format, ...) {
    if (PRINT_STATE == 2) return;

    va_list valist;
    va_start(valist, format);
    vprintf(format, valist);
}

void printd(const char* format, ...) {
    if (PRINT_STATE == 0) return;

    va_list valist;
    va_start(valist, format);
    vprintf(format, valist);
}

void printDelimiter(bool full) {
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        if (full) print("+---");
        else print("+   ");
    }
    print("+\n");
}

void printMatrix(char* matrix) {
    printDelimiter(true);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (!(i % 4)) print("| %c   ", matrix[i]);
        else if (!((i + 1) % 16)) {
            print("%c |\n", matrix[i]);
            printDelimiter(!((i + 1) % 64));
        }
        else if (!((i + 1) % 4)) print("%c ", matrix[i]);
        else print("%c   ", matrix[i]);
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
        printd("%d:%d - ", x, y);
        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (cell->posibs[j]) {
                countChars++;
                printd("%c, ", 'a' + j);
            }
        }
        count++;
        printd("\n");
    }
    printd("%d, %d\n", count, countChars);
}