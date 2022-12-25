#include "solver.h"


int isAloneInRow(uintptr_t* posCells, int index, int value) {
    int x, y;
    int count = 0;
    getCoordinates(index, &x, &y);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int ind = getIndex(i, y);
        CELL* cell = (CELL*) posCells[ind];
        if (cell->posibs[value]) count++;
    }
    return count;
}

int isAloneInCol(uintptr_t* posCells, int index, int value) {
    int x, y;
    int count = 0;
    getCoordinates(index, &x, &y);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int ind = getIndex(x, i);
        CELL* cell = (CELL*) posCells[ind];
        if (cell->posibs[value]) count++;
    }
    return count;
}

int isAloneInRect(uintptr_t* posCells, int index, int value) {
    int count = 0;
    int rect = getRect(index);
    int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int ind = getIndex(X, Y);

            CELL* cell = (CELL*) posCells[ind];
            if (cell->posibs[value]) count++;
        }
    }
    return count;
}

bool isAlone(uintptr_t* posCells, int index, int value) {
    return (isAloneInRow(posCells, index, value) == 1 ||
            isAloneInCol(posCells, index, value) == 1 ||
            isAloneInRect(posCells, index, value) == 1);

}

int fillOnePosibs(char* matrix, uintptr_t* posCells) {
    int changes = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') continue;

        CELL* cell = (CELL*) posCells[i];
        if (cell->count == 1) {
            changes++;

            int x, y;
            getCoordinates(i, &x, &y);
            for (int j = 0; j < MATRIX_LENGTH; j++) {
                if (cell->posibs[j]) {
                    matrix[i] = 'a' + j;
                    removePos(posCells, i, j);
                    break;
                }
            }
        }
    }
    return changes;
}

int fillLonelyPosibs(char* matrix, uintptr_t* posCells) {
    int changes = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') continue;

        CELL* cell = (CELL*) posCells[i];
        if (cell->count <= 0) continue;

        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (!cell->posibs[j]) continue;
            if (isAlone(posCells, i, j)) {
                changes++;
                matrix[i] = 'a' + j;
                removePos(posCells, i, j);
                break;
            }
        }
    }
    return changes;
}

int findLines(char* matrix, uintptr_t* posCells) {
    int changes = 0;

    for (int j = 0; j < MATRIX_LENGTH; j++) {
        /*
        Matrix 4x16 of posibilities in rows for each rectangle
        [0] is first rectangle first row,
        [1] is second rectangle first row
        [4] is first rectangle second row
        */
        int* rowsMatrix = (int*) malloc(sizeof(*rowsMatrix) * MATRIX_LENGTH * RECT_LENGTH);

        // Goes through every row (y value)
        for (int y = 0; y < MATRIX_LENGTH; y++) {
            // Goes through every rectangle (x value)
            for (int x = 0; x < RECT_LENGTH; x++) {
                int index = y * RECT_LENGTH + x; 
                rowsMatrix[index] = 0;

                for (int i = 0; i < RECT_LENGTH; i++) {
                    int ind = y * MATRIX_LENGTH + x * RECT_LENGTH + i;
                    CELL* cell = (CELL*) posCells[ind];
                    if (cell->posibs[j]) rowsMatrix[index]++;
                }
            }
        }

        printd("'%c' ===========\n", 'a' + j);
        for (int i = 0; i < MATRIX_LENGTH * RECT_LENGTH; i++) {
            printd("%d ", rowsMatrix[i]);
            if ((i+1) % 4 == 0) printd("\n");
            if ((i+1) % 16 == 0) printd("----\n");
        }

        /*
        Matrix 4x4 of counts of rows in rect
        */
        int* rowsCountPerColumn = (int*) malloc(sizeof(*rowsCountPerColumn) * MATRIX_LENGTH);
        for (int i = 0; i < MATRIX_LENGTH; i++) rowsCountPerColumn[i] = 0;

        for (int i = 0; i < MATRIX_LENGTH * RECT_LENGTH; i++) {
            int ind = (4 * (i / MATRIX_LENGTH)) + i % RECT_LENGTH;
            if (rowsMatrix[i]) rowsCountPerColumn[ind]++;
        } 

        for (int i = 0; i < MATRIX_LENGTH; i++) {
            printd("%d ", rowsCountPerColumn[i]);
        }
        printd("\n"); 

        for (int r = 0; r < RECT_LENGTH; r++) {
            for (int i = r*4; i < r + RECT_LENGTH - 1; i++) {
                int count = rowsCountPerColumn[i];
                int size = 0;
                /*
                Array of indexes which have same not-empty rows
                count of equalRows have to be same as count of
                not-empty rows
                */
                int* equalRows = (int*) malloc(sizeof(*equalRows) * count);
                for (int o = r; o < r + RECT_LENGTH; o++) {
                    if (count == rowsCountPerColumn[o]) {
                        equalRows[size++] = o;
                    }
                } 

                for (int o = 0; o < size; o++) {
                    
                }
            }
        }

        free(rowsCountPerColumn);
        free(rowsMatrix);
    }
    printd("=======\n");

    return changes;
}

int solve(char* matrix, int* iterations) {
    uintptr_t* posCells = (uintptr_t*) malloc(sizeof(*posCells) * MATRIX_SIZE);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        posCells[i] = (uintptr_t) initCell();
    }

    makePosibs(matrix, posCells);

    int iter = 0;
    int changes = 1;
    while (changes) {
        iter++;
        changes = 0;
        changes += fillOnePosibs(matrix, posCells);
        changes += fillLonelyPosibs(matrix, posCells);
        //if (iter == 1) changes += findLines(matrix, posCells);
    }

    *iterations += iter;
    int ret = analyze(matrix, posCells);

    if (ret >= 2) {
        int count = 0;
        int index = ret - 2;
        CELL* minCell = (CELL*) posCells[index];
        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (minCell->posibs[j]) {
                char* subMatrix = (char*) malloc(MATRIX_SIZE);
                for (int i = 0; i < MATRIX_SIZE; i++) subMatrix[i] = matrix[i];

                subMatrix[index] = 'a' + j;
                count += solve(subMatrix, iterations);
                free(subMatrix);
            }
        }
        ret = count;
    }
    
    freeCells(posCells);
    return ret;
}
