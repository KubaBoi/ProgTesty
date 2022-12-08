#ifndef PRINT_H
#define PRINT_H

#include "variables.h"
#include "help.h"

void printDelimiter(bool full);
void printMatrix(char* matrix);

void printPosCells(char* matrix, uintptr_t* posCells);

#endif