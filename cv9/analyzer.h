#ifndef ANALYZER_H
#define ANALYZER_H

#include "variables.h"
#include "help.h"


int findMax(char* matrix, uintptr_t* posCells);

/*
0 - done
1 - no solution
< - count of solutions
*/
int analyze(char* matrix, uintptr_t* posCells);

double percentage(char* matrix);

#endif