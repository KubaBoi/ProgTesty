#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include <limits.h>

#define CHAR_BLOCK 10
#define LINE_LENGTH 65
#define MATRIX_LENGTH 16
#define MATRIX_SIZE MATRIX_LENGTH*MATRIX_LENGTH

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

void getCoordinates(int index, int* x, int* y) {
    if (index >= MATRIX_SIZE) return;

    *x = index % MATRIX_LENGTH;
    *y = index / MATRIX_LENGTH;
}

int getIndex(int x, int y) {
    return y * MATRIX_LENGTH + x;
}

int fillMatrixLine(char* matrix, char* line, int lineIndex) {
    int startIndex = lineIndex * MATRIX_LENGTH;
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        int indexInLine = 2 + (i * 4);
        int index = startIndex + i;
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

void printMatrix(char* matrix) {
    for (int y = 0; y < MATRIX_LENGTH; y++) {
        for (int x = 0; x < MATRIX_LENGTH; x++) {
            printf(" '%c' ", matrix[getIndex(x, y)]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf(" '%c' ", matrix[i]);
        if (!((i + 1) % 16)) printf("\n");
    }
}

int main() {
    char* matrix = (char*) malloc(MATRIX_SIZE);
    if (readInput(matrix)) {
        printf("Nespravny vstup.");
        free(matrix);
        return 1;
    }
    
    printMatrix(matrix);
    
    free(matrix);
}