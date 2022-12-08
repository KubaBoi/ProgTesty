#include "input.h"

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