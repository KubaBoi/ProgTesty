#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include <limits.h>

#define CHAR_BLOCK 10
#define LINE_LENGTH 65
#define RECT_LENGTH 4
#define MATRIX_LENGTH 16
#define MATRIX_SIZE MATRIX_LENGTH*MATRIX_LENGTH

typedef struct cell {
    int count;
    bool posibs[MATRIX_LENGTH];
} CELL;

void nullString(char *str, int index, int size) {
	for (int i = index; i < size; i++) str[i] = 0;
}

int appendString(char *str, char *temp, int *filled) {
	int f = *filled;
	for (int i = 0; i < CHAR_BLOCK; i++) {
		if (temp[i] == '\n') {
			*filled = f;
			return 1;
		}

		str[f++] = temp[i];
		if (temp[i] == 0) f--;
	}
	*filled = f;
	return 0;
}

char* readLine(int* length) {
	int size = CHAR_BLOCK;
	char *string = (char *)malloc(size);
	int filled = 0;

	nullString(string, 0, size);

	char *newStr = (char *)malloc(CHAR_BLOCK);

	while (true) {
		nullString(newStr, 0, CHAR_BLOCK);

		if (fgets(newStr, sizeof(newStr), stdin) == NULL) {
			if (filled == 0) {
				free(string);
				free(newStr);
				return NULL;
			}
            *length = filled;
			break;
		}

		if (size < filled + CHAR_BLOCK) {
			size *= 2;
			string = (char *)realloc(string, size);
			nullString(string, filled, size);
		}

		if (appendString(string, newStr, &filled)) break;
	}

    *length = filled;
	free(newStr);
	return string;
}

int fillMatrixLine(char* matrix, char* line, int lineIndex) {
    int startIndex = lineIndex * MATRIX_LENGTH;
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int indexInLine = 2 + (i * 4);
        int index = startIndex + i;
        
        char c = line[indexInLine];
        if (c != ' ' && (c < 'a' || c > 'p')) return 1;

        matrix[index] = line[indexInLine];
    }
    return 0;
}

int readInput(char* matrix) {
    int length;
    int counter = 0;
    int lineIndex = 0;
    
    char* line = readLine(&length);
    while (line) {
        counter++;
        if (length != LINE_LENGTH) return 1;

        if (!(counter % 2)) {
            if (fillMatrixLine(matrix, line, lineIndex++)) return 1;
        }

        free(line);
        line = readLine(&length);
    }
    if (counter != 33) return 1;

    return 0;
}

// PRINT

void printDelimiter(bool full) {
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        if (full) printf("+---");
        else printf("+   ");
    }
    printf("+\n");
}

void printMatrix(char* matrix) {
    printDelimiter(true);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (!(i % 4)) printf("| %c   ", matrix[i]);
        else if (!((i + 1) % 16)) {
            printf("%c |\n", matrix[i]);
            printDelimiter(!((i + 1) % 64));
        }
        else if (!((i + 1) % 4)) printf("%c ", matrix[i]);
        else printf("%c   ", matrix[i]);
    }
}

// HELP

void getCoordinates(int index, int* x, int* y) {
    if (index >= MATRIX_SIZE) return;

    *x = index % MATRIX_LENGTH;
    *y = index / MATRIX_LENGTH;
}

int getIndex(int x, int y) {
    return y * MATRIX_LENGTH + x;
}

int getCharValue(char c) {
    if (c == ' ') return MATRIX_LENGTH;
    return c - 'a'; // 97
}

int getRect(int index) {
    int x, y;
    getCoordinates(index, &x, &y);
    return (x / 4) + (y / 4) * 4;
}

CELL* initCell() {
    CELL* cell = (CELL*) malloc(sizeof(*cell));
    cell->count = 0;
    for (int i = 0; i < MATRIX_LENGTH; i++) cell->posibs[i] = false;
    return cell;
}

void freeCells(uintptr_t* cells) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) cells[i];
        free(cell);
    }
    free(cells);
}

// CHEKING

bool* makeBooleanArray() {
    bool* array = (bool*) malloc(MATRIX_LENGTH+1);
    for (int i = 0; i < MATRIX_LENGTH+1; i++) array[i] = false;
    return array;
}

bool isPartOk(char* matrix, int x, int y, bool* arr) {
    int index = getIndex(x, y);
    int value = getCharValue(matrix[index]);
    if (arr[value] && value < MATRIX_LENGTH) return false;
    arr[value] = true;
    return true;
}

bool isRectangleOk(char* matrix, int number) {
    bool ret = true;
    bool* rectArray = makeBooleanArray();

    int xConst = (number % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (number / RECT_LENGTH) * RECT_LENGTH;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int index = getIndex(X, Y);
            int value = getCharValue(matrix[index]);
            
            if (rectArray[value] && value < MATRIX_LENGTH) {
                ret = false;
                break;
            }
            rectArray[value] = true;
        }
    }
    free(rectArray);
    return ret;
}

bool isMatrixOk(char* matrix) {
    bool* rowArray;
    bool* colArray;

    for (int y = 0; y < MATRIX_LENGTH; y++) {
        bool ret = true;
        rowArray = makeBooleanArray();
        colArray = makeBooleanArray();
        for (int x = 0; x < MATRIX_LENGTH; x++) {
            ret = isPartOk(matrix, x, y, rowArray);
            if (!ret) break;

            ret = isPartOk(matrix, y, x, colArray);
            if (!ret) break;
        }
        free(rowArray);
        free(colArray);
        if (!ret) return false;
    }

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        if (!isRectangleOk(matrix, i)) return false;
    }
    return true;
}

bool isDone(char* matrix) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] == ' ') return false;
    }
    return true;
}

// POSIBILITIES

void getPartOfPosibs(char* matrix, int constant, bool isX, bool* arr) {
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int index;
        if (isX) index = getIndex(constant, i);
        else index = getIndex(i, constant);

        int value = getCharValue(matrix[index]);
        arr[value] = true;
    }
}

void getRectOfPosibs(char* matrix, int number, bool* arr) {
    int xConst = (number % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (number / RECT_LENGTH) * RECT_LENGTH;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int index = getIndex(X, Y);

            int value = getCharValue(matrix[index]);
            arr[value] = true;
        }
    }
}

void makeOnePosib(char* matrix, uintptr_t* posCells, int index) {
    CELL* cell = (CELL*) posCells[index];
    bool* array = makeBooleanArray();
        
    int X, Y, rect;
    getCoordinates(index, &X, &Y);
    rect = getRect(index);
    
    getPartOfPosibs(matrix, Y, false, array);
    getPartOfPosibs(matrix, X, true, array);
    getRectOfPosibs(matrix, rect, array);
    
    for (int j = 0; j < MATRIX_LENGTH; j++) {
        if (!array[j] && !cell->posibs[j]) {
            cell->count++;
            cell->posibs[j] = true;
        }
        else if (array[j] && cell->posibs[j]) {
            cell->count--;
            cell->posibs[j] = false;
        }
    }
    
    free(array);
}

void makePosibs(char* matrix, uintptr_t* posCells) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') {
            cell->count = 0;
            continue;
        }
        
        makeOnePosib(matrix, posCells, i);
    }
}

int removePosInRow(uintptr_t* posCells, int index, int value) {
    int x, y;
    getCoordinates(index, &x, &y);
    int counter = 0;

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int indY = getIndex(i, y);
        CELL* cellY = (CELL*) posCells[indY];

        if (cellY->posibs[value]) {
            cellY->posibs[value] = false;
            cellY->count--;
            counter++;
        }
    }
    return counter;
}

int removePosInCol(uintptr_t* posCells, int index, int value) {
    int x, y;
    getCoordinates(index, &x, &y);
    int counter = 0;

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int indX = getIndex(x, i);
        CELL* cellX = (CELL*) posCells[indX];

        if (cellX->posibs[value]) {
            cellX->posibs[value] = false;
            cellX->count--;
            counter++;
        }
    }
    return counter;
}

int removePosInRect(uintptr_t* posCells, int index, int value) {
    int rect = getRect(index);
    int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;
    int counter = 0;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int ind = getIndex(X, Y);

            CELL* cell = (CELL*) posCells[ind];
            if (cell->posibs[value]) {
                cell->posibs[value] = false;
                cell->count--;
                counter++;
            }
        }
    }
    return counter;
}

int removePos(uintptr_t* posCells, int index, int value) {
    int counter = 0;
    counter += removePosInRow(posCells, index, value);
    counter += removePosInCol(posCells, index, value);
    counter += removePosInRect(posCells, index, value);

    return counter;
}

// SOLVER

int isAloneInRow(uintptr_t* posCells, int index, int value) {
    int x, y;
    int count = 0;
    getCoordinates(index, &x, &y);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int ind = getIndex(i, y);
        CELL* cell = (CELL*) posCells[ind];
        if (cell->posibs[value]) count++;
    }
    return count;
}

int isAloneInCol(uintptr_t* posCells, int index, int value) {
    int x, y;
    int count = 0;
    getCoordinates(index, &x, &y);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int ind = getIndex(x, i);
        CELL* cell = (CELL*) posCells[ind];
        if (cell->posibs[value]) count++;
    }
    return count;
}

int isAloneInRect(uintptr_t* posCells, int index, int value) {
    int count = 0;
    int rect = getRect(index);
    int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
    int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;

    for (int y = 0; y < RECT_LENGTH; y++) {
        for (int x = 0; x < RECT_LENGTH; x++) {
            int X = x + xConst;
            int Y = y + yConst;
            int ind = getIndex(X, Y);

            CELL* cell = (CELL*) posCells[ind];
            if (cell->posibs[value]) count++;
        }
    }
    return count;
}

bool isAlone(uintptr_t* posCells, int index, int value) {
    return (isAloneInRow(posCells, index, value) == 1 ||
            isAloneInCol(posCells, index, value) == 1 ||
            isAloneInRect(posCells, index, value) == 1);

}

int fillOnePosibs(char* matrix, uintptr_t* posCells) {
    int changes = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') continue;

        CELL* cell = (CELL*) posCells[i];
        if (cell->count == 1) {
            changes++;

            int x, y;
            getCoordinates(i, &x, &y);
            for (int j = 0; j < MATRIX_LENGTH; j++) {
                if (cell->posibs[j]) {
                    matrix[i] = 'a' + j;
                    removePos(posCells, i, j);
                    break;
                }
            }
        }
    }
    return changes;
}

int fillLonelyPosibs(char* matrix, uintptr_t* posCells) {
    int changes = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') continue;

        CELL* cell = (CELL*) posCells[i];
        if (cell->count <= 0) continue;

        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (!cell->posibs[j]) continue;
            if (isAlone(posCells, i, j)) {
                changes++;
                matrix[i] = 'a' + j;
                removePos(posCells, i, j);
                break;
            }
        }
    }
    return changes;
}

int findLines(char* matrix, uintptr_t* posCells) {
    int changes = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (matrix[i] != ' ') continue;

        CELL* cell = (CELL*) posCells[i];
        if (cell->count <= 0) continue;

        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (!cell->posibs[j]) continue;
            int countInRect = isAloneInRect(posCells, i, j);
            
            int countInRow = 0;
            int countInCol = 0;
            
            int X, Y;
            getCoordinates(i, &X, &Y);

            int rect = getRect(i);
            int xConst = (rect % RECT_LENGTH) * RECT_LENGTH;
            int yConst = (rect / RECT_LENGTH) * RECT_LENGTH;

            uintptr_t* items = (uintptr_t*) malloc(sizeof(*items) * countInRect);
            for (int y = 0; y < RECT_LENGTH; y++) {
                for (int x = 0; x < RECT_LENGTH; x++) {
                    int xP = x + xConst;
                    int yP = y + yConst;
                    int ind = getIndex(xP, yP);
                    
                    CELL* cellP = (CELL*) posCells[ind];

                    if (!cellP->posibs[j]) continue;

                    if (xP == X) countInRow++;
                    else if (yP == Y) countInCol++;
                    else continue;

                    items[(countInRow + countInCol) - 1] = (uintptr_t) cellP;
                }
            }

            if (countInRow == countInRect) {
                changes += removePosInRow(posCells, i, j) - countInRow;
                for (int o = 0; o < countInRow; o++) {
                    CELL* remCell = (CELL*) items[o];
                    remCell->count++;
                    remCell->posibs[j] = true;
                }
            }
            else if (countInCol == countInRect) {
                changes += removePosInCol(posCells, i, j) - countInCol;
                for (int o = 0; o < countInCol; o++) {
                    CELL* remCell = (CELL*) items[o];
                    remCell->count++;
                    remCell->posibs[j] = true;
                }
            }
            free(items);
        }
    }
    return changes;
}

// ANALYZER

void printPosCells(char* matrix, uintptr_t* posCells) {
    int count = 0;
    int countChars = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') continue;

        int x, y;
        getCoordinates(i, &x, &y);
        printf("%d:%d - ", x, y);
        for (int j = 0; j < MATRIX_LENGTH; j++) {
            if (cell->posibs[j]) {
                countChars++;
                printf("%c, ", 'a' + j);
            }
        }
        count++;
        printf("\n");
    }
    printf("%d, %d\n", count, countChars);
}

/*
0 - done
1 - no solution
< - count of solutions
*/
int analyze(char* matrix, uintptr_t* posCells) {
    int filled = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CELL* cell = (CELL*) posCells[i];
        if (matrix[i] != ' ') {
            filled++;
            continue;  
        }

        if (cell->count == 0) return 1;
    }

    if (filled == MATRIX_SIZE) return 0;
    return 2;
}

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

    printPosCells(matrix, posCells);

    if (iter >= 100000) {
        printMatrix(matrix);
        return 1;
    }

    int result = analyze(matrix, posCells);
    switch (result)
    {
    case 0:
        printMatrix(matrix);
        break;
    case 1:
        printf("Reseni neexistuje.\n");
        break;
    default:
        printMatrix(matrix);
        break;
    }
    printf("iterations: %d\n", iter);

    freeCells(posCells);
    free(matrix);
}