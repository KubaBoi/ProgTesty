#ifndef POSIBILITIES_H
#define POSIBILITIES_H

#include "variables.h"
#include "help.h"
#include "removes.h"

void getPartOfPosibs(char* matrix, int constant, bool isX, bool* arr);
void getRectOfPosibs(char* matrix, int number, bool* arr);

void makeOnePosib(char* matrix, uintptr_t* posCells, int index);
void makePosibs(char* matrix, uintptr_t* posCells);

#endif