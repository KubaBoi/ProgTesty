#include <stdio.h>
#include <cstdlib>
#include <cstdint>

#include "variables.h"
#include "input.h"
#include "print.h"
#include "help.h"
#include "checks.h"
#include "posibilities.h"
#include "solver.h"
#include "analyzer.h"

int main() {
    char* matrix = (char*) malloc(MATRIX_SIZE);
    uintptr_t* posCells = (uintptr_t*) malloc(sizeof(*posCells) * MATRIX_SIZE);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        posCells[i] = (uintptr_t) initCell();
    }

    printf("Zadejte hexadoku:\n");

    if (readInput(matrix) || !isMatrixOk(matrix)) {
        printf("Nespravny vstup.\n");
        freeCells(posCells);
        free(matrix);
        return 1;
    }
    
    double tempPerc = percentage(matrix);
    makePosibs(matrix, posCells);

    int changes = 1;
    int iter = 0;
    while (changes && iter < 100000) {
        changes = 0;
        changes += fillOnePosibs(matrix, posCells);
        changes += fillLonelyPosibs(matrix, posCells);
        changes += findLines(matrix, posCells);
        iter++;
    }

    //printPosCells(matrix, posCells);

    int result = analyze(matrix, posCells);
    switch (result)
    {
    case 0:
        printMatrix(matrix);
        break;
    case 1:
        print("Reseni neexistuje.\n");
        break;
    default:
        printMatrix(matrix);
        break;
    }
    printd("temp prcnt: %lf\n", tempPerc);
    printd("percetange: %lf\n", percentage(matrix));
    printd("iterations: %d\n", iter);

    findTwoLines(matrix, posCells);

    freeCells(posCells);
    free(matrix);
}