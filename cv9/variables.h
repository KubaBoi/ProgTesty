#ifndef VARIABLES_H
#define VARIABLES_H

/*
0 - only wanted ouput
1 - debug
2 - only debug
*/
#define PRINT_STATE 0

#define CHAR_BLOCK 10
#define LINE_LENGTH 65
#define RECT_LENGTH 4
#define MATRIX_LENGTH 16
#define MATRIX_SIZE MATRIX_LENGTH*MATRIX_LENGTH

typedef struct cell {
    int count;
    bool posibs[MATRIX_LENGTH];
} CELL;

#endif