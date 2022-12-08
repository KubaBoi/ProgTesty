#ifndef REMOVES_H
#define REMOVES_H

#include "variables.h"
#include "help.h"

int removePosInRow(uintptr_t* posCells, int index, int value);
int removePosInCol(uintptr_t* posCells, int index, int value);
int removePosInRect(uintptr_t* posCells, int index, int value);
int removePos(uintptr_t* posCells, int index, int value);

#endif