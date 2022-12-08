#ifndef SOLVER_H
#define SOLVER_H

#include <cstring>

#include "variables.h"
#include "help.h"
#include "posibilities.h"
#include "print.h"
#include "analyzer.h"

int isAloneInRow(uintptr_t* posCells, int index, int value);
int isAloneInCol(uintptr_t* posCells, int index, int value);
int isAloneInRect(uintptr_t* posCells, int index, int value);
bool isAlone(uintptr_t* posCells, int index, int value);

int fillOnePosibs(char* matrix, uintptr_t* posCells);
int fillLonelyPosibs(char* matrix, uintptr_t* posCells);

int findLines(char* matrix, uintptr_t* posCells);
int findTwoLines(char* matrix, uintptr_t* posCells);

int solve(char* matrix);

#endif