#include "removes.h"

int removePosInRow(uintptr_t* posCells, int index, int value) {
    int x, y;
    getCoordinates(index, &x, &y);
    int counter = 0;

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int indY = getIndex(i, y);
        CELL* cellY = (CELL*) posCells[indY];

        if (cellY->posibs[value]) {
            cellY->posibs[value] = false;
            cellY->count--;
            counter++;
        }
    }
    return counter;
}

int removePosInCol(uintptr_t* posCells, int index, int value) {
    int x, y;
    getCoordinates(index, &x, &y);
    int counter = 0;

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int indX = getIndex(x, i);
        CELL* cellX = (CELL*) posCells[indX];

        if (cellX->posibs[value]) {
            cellX->posibs[value] = false;
            cellX->count--;
            counter++;
        }
    }
    return counter;
}

int removePosInRect(uintptr_t* posCells, int index, int value) {
    int rect = getRect(index);
    int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;
    int counter = 0;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int ind = getIndex(X, Y);

            CELL* cell = (CELL*) posCells[ind];
            if (cell->posibs[value]) {
                cell->posibs[value] = false;
                cell->count--;
                counter++;
            }
        }
    }
    return counter;
}

int removePos(uintptr_t* posCells, int index, int value) {
    int counter = 0;
    counter += removePosInRow(posCells, index, value);
    counter += removePosInCol(posCells, index, value);
    counter += removePosInRect(posCells, index, value);

    CELL* cell = (CELL*) posCells[index];
    cell->count = 0;
    for (int i = 0; i < MATRIX_LENGTH; i++) cell->posibs[i] = false; 

    return counter;
}