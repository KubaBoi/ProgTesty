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

    printf("Zadejte hexadoku:\n");
    printd("Testing input\n");
    if (readInput(matrix) || !isMatrixOk(matrix)) {
        printf("Nespravny vstup.\n");
        free(matrix);
        return 1;
    }
    printd("Input ok\n");
    
    double tempPerc = percentage(matrix);

    int iterations = 0;
    int result = solve(matrix, &iterations);

    switch (result)
    {
    case 1:
        printMatrix(matrix);
        break;
    case 0:
        print("Reseni neexistuje.\n");
        break;
    default:
        print("Celkem reseni: %d\n", result);
        break;
    }
    printd("temp prcnt: %lf\n", tempPerc);
    printd("percetange: %lf\n", percentage(matrix));
    printd("Iterations: %d\n", iterations);

    free(matrix);
}