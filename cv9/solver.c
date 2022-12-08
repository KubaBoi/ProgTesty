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

            uintptr_t* items = (uintptr_t*) malloc(sizeof(*items) * countInRect);
            for (int o = 0; o < countInRect; o++) items[o] = 0;

            for (int o = 0; o < RECT_LENGTH; o++) {
                int xP = o + xConst;
                int yP = o + yConst;
                
                int indX = getIndex(xP, Y);
                int indY = getIndex(X, yP);

                CELL* cellX = (CELL*) posCells[indX];
                CELL* cellY = (CELL*) posCells[indY];

                if (cellX->posibs[j]) items[countInRow++] = (uintptr_t) cellX;
                else if (cellY->posibs[j]) items[countInCol++] = (uintptr_t) cellY;
            }

            if (countInRow == countInRect) {
                changes += removePosInRow(posCells, i, j) - countInRow;
                printf("%d - %d, %d\n", countInRow, i, rect);
                printf("%c -> ", 'a' + j);
                for (int o = 0; o < MATRIX_LENGTH; o++) {
                    CELL* cell = (CELL*) posCells[i / MATRIX_LENGTH + o];
                    printf("%d ", cell->posibs[j]);
                }
                printf("\n");
            }
            else if (countInCol == countInRect) {
                changes += removePosInCol(posCells, i, j) - countInCol;
            }
            else {
                free(items);
                continue;
            }

            for (int o = 0; o < countInRect; o++) {
                CELL* remCell = (CELL*) items[o];
                if (remCell && !remCell->posibs[j]) {
                    remCell->count++;
                    remCell->posibs[j] = true;
                }
            }
            if (countInRow == countInRect) {
                printf("%c -> ", 'a' + j);
                for (int o = 0; o < MATRIX_LENGTH; o++) {
                    CELL* cell = (CELL*) posCells[i / MATRIX_LENGTH + o];
                    printf("%d ", cell->posibs[j]);
                }
                printf("\n\n");
            }
            free(items);
        }
    }
    return changes;
}