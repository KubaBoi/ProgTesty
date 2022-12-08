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
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') continue;

        if (i > 20) return changes;

        CELL* cell = (CELL*) posCells[i];
        if (cell->count <= 0) continue;

        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (!cell->posibs[j]) continue;
            int countInRect = isAloneInRect(posCells, i, j);
            
            int countInRow = 0;
            int countInCol = 0;
            
            int X, Y;
            getCoordinates(i, &X, &Y);

            int rect = getRect(i);
            int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
            int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;

            int* items = (int*) malloc(sizeof(*items) * countInRect);
            for (int o = 0; o < countInRect; o++) items[o] = 0;

            for (int o = 0; o < RECT_LENGTH; o++) {
                int xP = o + xConst;
                int yP = o + yConst;

                int indX = getIndex(xP, Y);
                int indY = getIndex(X, yP);

                CELL* cellX = (CELL*) posCells[indX];
                CELL* cellY = (CELL*) posCells[indY];

                if (cellX->posibs[j]) items[countInRow++] = indX;
                else if (cellY->posibs[j]) items[countInCol++] = indY;
            }

            if (countInRow == countInRect) {
                changes += removePosInRow(posCells, i, j);
            }
            else if (countInCol == countInRect) {
                changes += removePosInCol(posCells, i, j);
            }
            else {
                free(items);
                continue;
            }

            changes -= countInRect;

            for (int o = 0; o < countInRect; o++) {
                CELL* cell = (CELL*) posCells[items[o]];
                cell->posibs[j] = true;
                cell->count++;
            }
            free(items);
        }
    }
    return changes;
}

int findTwoLines(char* matrix, uintptr_t* posCells) {
    int changes = 0;

    for (int j = 0; j < MATRIX_LENGTH; j++) {
        uintptr_t* matrices = (uintptr_t*) malloc(sizeof(*matrices) * MATRIX_LENGTH);
        for (int i = 0; i < MATRIX_LENGTH; i++) {
            int multiplier = 1;
            for (int o = 0; o < RECT_LENGTH; o++) multiplier *= RECT_LENGTH;
            int index = (RECT_LENGTH * (i % RECT_LENGTH)) + ((i / RECT_LENGTH) * multiplier);
            
            int rect = getRect(index);
            int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
            int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;
            
            int* rectMatrix = (int*) malloc(sizeof(*rectMatrix) * RECT_LENGTH * 2);
            for (int o = 0; o < RECT_LENGTH * 2; o++) rectMatrix[o] = 0;

            for (int y = 0; y < RECT_LENGTH; y++) {
                for (int x = 0; x < RECT_LENGTH; x++) {
                    int X = x + xConst;
                    int Y = y + yConst;
                    int ind = getIndex(X, Y);

                    CELL* cell = (CELL*) posCells[ind];
                    printd("%d") // zkusim vypsat cell a uvidim jestli to matchuje
                    // mozna je problem v prvnim makePoses
                    rectMatrix[y] += cell->posibs[j];
                    rectMatrix[RECT_LENGTH + x] += cell->posibs[j];
                }
            }
            for (int o = 0; o < RECT_LENGTH; o++) {
                printd("%d ", rectMatrix[o]);
            }
            printd("    ");
            for (int o = RECT_LENGTH; o < RECT_LENGTH * 2; o++) {
                printd("%d ", rectMatrix[o]);
            }
            printd("\n");
            break;
            matrices[i] = (uintptr_t) rectMatrix;
        }
        printd("\n");
        break;



        for (int i = 0; i < MATRIX_LENGTH; i++) {
            free((int*) matrices[i]);
        }
        free(matrices);
    }
}


