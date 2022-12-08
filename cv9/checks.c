#include "checks.h"

bool isPartOk(char* matrix, int x, int y, bool* arr) {
    int index = getIndex(x, y);
    int value = getCharValue(matrix[index]);
    if (arr[value] && value < MATRIX_LENGTH) return false;
    arr[value] = true;
    return true;
}

bool isRectangleOk(char* matrix, int number) {
    bool ret = true;
    bool* rectArray = makeBooleanArray();

    int xConst = (number % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (number / RECT_LENGTH) * RECT_LENGTH;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int index = getIndex(X, Y);
            int value = getCharValue(matrix[index]);
            
            if (rectArray[value] && value < MATRIX_LENGTH) {
                ret = false;
                break;
            }
            rectArray[value] = true;
        }
    }
    free(rectArray);
    return ret;
}

bool isMatrixOk(char* matrix) {
    bool* rowArray;
    bool* colArray;

    for (int y = 0; y < MATRIX_LENGTH; y++) {
        bool ret = true;
        rowArray = makeBooleanArray();
        colArray = makeBooleanArray();
        for (int x = 0; x < MATRIX_LENGTH; x++) {
            ret = isPartOk(matrix, x, y, rowArray);
            if (!ret) break;

            ret = isPartOk(matrix, y, x, colArray);
            if (!ret) break;
        }
        free(rowArray);
        free(colArray);
        if (!ret) return false;
    }

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        if (!isRectangleOk(matrix, i)) return false;
    }
    return true;
}