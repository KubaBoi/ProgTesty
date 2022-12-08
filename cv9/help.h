#ifndef HELP_H
#define HELP_H

#include <stdio.h>
#include <cstdint>
#include <cstdlib>

#include "variables.h"

void getCoordinates(int index, int* x, int* y);

int getIndex(int x, int y);

int getCharValue(char c);

int getRect(int index);

CELL* initCell();

void freeCells(uintptr_t* cells);

bool* makeBooleanArray();

#endif