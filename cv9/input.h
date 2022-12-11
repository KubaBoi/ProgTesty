#ifndef INPUT_H
#define INPUT_H

#include <cstdlib>
#include <cstdio>

#include "variables.h"
#include "print.h"

void nullString(char *str, int index, int size);

int appendString(char *str, char *temp, int *filled);

char* readLine(int* length);

int fillMatrixLine(char* matrix, char* line, int lineIndex);

int readInput(char* matrix);

#endif