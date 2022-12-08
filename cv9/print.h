#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>

#include "variables.h"
#include "help.h"

void print(const char* format, ...);
void printd(const char* format, ...);

void printDelimiter(bool full);
void printMatrix(char* matrix);

void printPosCells(char* matrix, uintptr_t* posCells);

#endif