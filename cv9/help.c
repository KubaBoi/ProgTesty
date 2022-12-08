#include "help.h"

void getCoordinates(int index, int* x, int* y) {
    if (index >= MATRIX_SIZE) return;

    *x = index % MATRIX_LENGTH;
    *y = index / MATRIX_LENGTH;
}

int getIndex(int x, int y) {
    return y * MATRIX_LENGTH + x;
}

int getCharValue(char c) {
    if (c == ' ') return MATRIX_LENGTH;
    return c - 'a'; // 97
}

int getRect(int index) {
    int x, y;
    getCoordinates(index, &x, &y);
    return (x / 4) + (y / 4) * 4;
}

CELL* initCell() {
    CELL* cell = (CELL*) malloc(sizeof(*cell));
    cell->count = 0;
    for (int i = 0; i < MATRIX_LENGTH; i++) cell->posibs[i] = false;
    return cell;
}

void freeCells(uintptr_t* cells) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) cells[i];
        free(cell);
    }
    free(cells);
}

bool* makeBooleanArray() {
    bool* array = (bool*) malloc(MATRIX_LENGTH+1);
    for (int i = 0; i < MATRIX_LENGTH+1; i++) array[i] = false;
    return array;
}