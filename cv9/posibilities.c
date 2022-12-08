#include "posibilities.h"

void getPartOfPosibs(char* matrix, int constant, bool isX, bool* arr) {
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int index;
        if (isX) index = getIndex(constant, i);
        else index = getIndex(i, constant);

        int value = getCharValue(matrix[index]);
        arr[value] = true;
    }
}

void getRectOfPosibs(char* matrix, int number, bool* arr) {
    int xConst = (number % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (number / RECT_LENGTH) * RECT_LENGTH;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int index = getIndex(X, Y);

            int value = getCharValue(matrix[index]);
            arr[value] = true;
        }
    }
}

void makeOnePosib(char* matrix, uintptr_t* posCells, int index) {
    CELL* cell = (CELL*) posCells[index];
    bool* array = makeBooleanArray();
        
    int X, Y, rect;
    getCoordinates(index, &X, &Y);
    rect = getRect(index);
    
    getPartOfPosibs(matrix, Y, false, array);
    getPartOfPosibs(matrix, X, true, array);
    getRectOfPosibs(matrix, rect, array);
    
    for (int j = 0; j < MATRIX_LENGTH; j++) {
        if (!array[j] && !cell->posibs[j]) {
            cell->count++;
            cell->posibs[j] = true;
        }
        else if (array[j] && cell->posibs[j]) {
            cell->count--;
            cell->posibs[j] = false;
        }
    }
    
    free(array);
}

void makePosibs(char* matrix, uintptr_t* posCells) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') {
            cell->count = 0;
            continue;
        }
        
        makeOnePosib(matrix, posCells, i);
    }
}