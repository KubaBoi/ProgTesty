#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include <limits.h>

#define CHAR_BLOCK 10
#define WING_COUNT 4  
#define MAX_REC 15

typedef struct wing {
    char name;
    int size, count;
    int* items;
} WING;

typedef struct tree {
    int selfBranchIndex, index, value;
    unsigned int depth;
    uintptr_t parent, wing;
    uintptr_t branches[4];
} TREE;

void nullString(char *str, int index, int size) {
	for (int i = index; i < size; i++) str[i] = 0;
}

void appendString(char *str, char *temp, int *filled) {
	int f = *filled;
	for (int i = 0; i < CHAR_BLOCK; i++) {
		str[f++] = temp[i];
		if (temp[i] == 0) f--;
	}
	*filled = f;
}

char* readLines() {
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
			break;
		}

		if (size < filled + CHAR_BLOCK) {
			size *= 2;
			string = (char *)realloc(string, size);
			nullString(string, filled, size);
		}

		appendString(string, newStr, &filled);
	}

	free(newStr);
	return string;
}

void removeWhiteSpaces(char* str) {
    int len = strlen(str);

    int index = 0;
    for (int i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            str[index++] = str[i];
        }
    }
    str[index] = 0;
}

void countItems(char* str, int* count) {
    int len = strlen(str);

    int ct = 0;
    bool counting = false;
    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            if (!counting) ct++;
            counting = true;
        }
        else {
            counting = false;
        }
    }
    *count = ct;
}

char* readInput(int* counts) {
    char* str = readLines();
    if (str) {
        countItems(str, counts);
        removeWhiteSpaces(str);
    }
    return str;
}

WING* initWing() {
    WING* wing = (WING*) malloc(sizeof(*wing));
    wing->name = 0;
    wing->size = CHAR_BLOCK;
    wing->count = 0;
    wing->items = (int*) malloc(sizeof(*wing->items) * wing->size);
    return wing;
}

char* loadNumber(char* str, char* number, int size) {
    int filled = 0;
    nullString(number, 0, size);

    if (*str == '-') {
        number[filled++] = '-';
        str++;
    }

    while (isdigit(*str) && *str) {
        number[filled++] = *str;

        if (size <= filled) {
            size *= 2;
            number = (char*) realloc(number, size);
            nullString(number, filled, size);
        }
        str++;
    }   
    if (!filled) return 0; // empty string
    return str;
}

bool isValidName(char str, uintptr_t* wings) {
    char names[] = {'N', 'W', 'E', 'S'};
    bool ok = false;
    
    for (int i = 0; i < WING_COUNT; i++) {
        if (str == names[i]) {
            ok = true;
            break;
        }
    }
    if (!ok) return false;

    for (int i = 0; i < WING_COUNT; i++) {
        if (!wings[i]) return true;

        WING* wing = (WING*) wings[i];
        if (wing->name == str) return false;
    }
    return true;
}

int loadInput(uintptr_t* wings, char* str) {

    int index = 0;
    while (*str) {
        if (isalpha(*str)) {
            if (!isValidName(*str, wings)) return 1;

            WING* wing = initWing();
            wing->name = *str;
            wings[index++] = (uintptr_t) wing;
            
            str++;
            if (*str != ':') return 1;
            str++;
            if (*str != '{') return 1;
            str++;
            
            while (true) {
                int size = CHAR_BLOCK;
                char* number = (char*) malloc(size);
                str = loadNumber(str, number, size);
                if (!str) return 1; // empty string
                
                wing->items[wing->count++] = atoi(number);
                if (wing->size <= wing->count) {
                    wing->size *= 2;
                    wing->items = (int*) realloc(wing->items, sizeof(*wing->items) * wing->size);
                }
                free(number);
                
                if (*str == '}') break;
                else if (*str != ',') return 1; 
                str++;
            }
        }
        else return 1;
        str++;
    }
    return 0;
}

void printWings(uintptr_t* wings) {
    for (int i = 0; i < WING_COUNT; i++) {
        WING* wing = (WING*) wings[i];
        printf("%c: ", wing->name);
        for (int j = 0; j < wing->count; j++) {
            printf("%d,", wing->items[j]);
        }
        printf("\n");
    }
}

void nullWings(uintptr_t* wings) {
    for (int i = 0; i < WING_COUNT; i++) {
        wings[i] = 0;
    }
}

void freeWings(uintptr_t* wings) {
    for (int i = 0; i < WING_COUNT; i++) {
        if (!wings[i]) break;
        WING* wing = (WING*) wings[i];
        free(wing->items);
        free(wing);
    }
    free(wings);
}

void freeTree(TREE* tree, int expection) {
    for (int i = 0; i < WING_COUNT; i++) {
        if (expection == i) continue;
        if (tree->branches[i]) freeTree((TREE*) tree->branches[i], -1);
    }
    free(tree);
}

void makeTree(TREE* tree, uintptr_t* wings, int* searchedIndexes, unsigned int endDepth) {
    
    if (endDepth <= tree->depth) return;

    for (int i = 0; i < WING_COUNT; i++) {
        int sIndex = searchedIndexes[i];
        WING* wing = (WING*) wings[i];

        int value = tree->value;
        if (tree->depth % 2) value -= wing->items[sIndex];
        else value += wing->items[sIndex];

        if (wing->count <= sIndex) continue;

        if (!tree->branches[i]) {
            TREE* newBranch = (TREE*) malloc(sizeof(*newBranch));
            newBranch->depth = tree->depth+1;
            newBranch->index = sIndex;
            newBranch->selfBranchIndex = i;
            newBranch->value = value;
            newBranch->parent = (uintptr_t) tree;
            newBranch->wing = (uintptr_t) wing;
            
            int sIndxs[4];
            for (int j = 0; j < WING_COUNT; j++) {
                newBranch->branches[j] = 0;
                sIndxs[j] = searchedIndexes[j];
            }
            sIndxs[i]++;

            makeTree(newBranch, wings, sIndxs, endDepth);
            tree->branches[i] = (uintptr_t) newBranch;
        } 
        else {
            TREE* existBranch = (TREE*) tree->branches[i];
            
            int sIndxs[4];
            for (int j = 0; j < WING_COUNT; j++) {
                sIndxs[j] = searchedIndexes[j];
            }
            sIndxs[i]++;
            
            makeTree(existBranch, wings, sIndxs, endDepth);
        }
    }
}

bool isLast(TREE* tree) {
    for (int i = 0; i < WING_COUNT; i++) {
        if (tree->branches[i]) return false;
    }
    return true;
}

void printBranch(TREE* tree) {
    char player = 'B';
    if (tree->depth % 2) {
        player = 'A';
    }

    WING* wing = (WING*) tree->wing;
    int value = wing->items[tree->index];

    printf("%c: %c[%d] (%d)\n", player, wing->name, tree->index, value);
}

int findMaxIndex(int* arr) {
    int m = INT_MIN;
    int index = 0;
    for (int i = 0; i < WING_COUNT; i++) {
        if (m < arr[i]) {
            m = arr[i];
            index = i;
        }
    }
    return index;
}

int findMinIndex(int* arr) {
    int m = INT_MAX;
    int index = 0;
    for (int i = 0; i < WING_COUNT; i++) {
        if (m > arr[i]) {
            m = arr[i];
            index = i;
        }
    }
    return index;
}

int findBestBranch(TREE* tree, unsigned int depth) {    

    if (isLast(tree)) return tree->value;

    int results[4];
    for (int i = 0; i < WING_COUNT; i++) {
        if (tree->branches[i]) {
            results[i] = findBestBranch((TREE*) tree->branches[i], depth);
        }
        else if (tree->depth % 2) results[i] = INT_MAX;
        else results[i] = INT_MIN;
    }

    if (tree->depth != depth) {
        if (tree->depth % 2) return results[findMinIndex(results)];
        return results[findMaxIndex(results)];
    }
    else {
        if (tree->depth % 2) return findMinIndex(results);
        return findMaxIndex(results);
    }
}

int main() {
    printf("Zetony:\n");
    int counts = 0;
    char* str = readInput(&counts);

    if (!str) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    uintptr_t* wings = (uintptr_t*) malloc(sizeof(uintptr_t) * WING_COUNT);
    nullWings(wings);

    if (loadInput(wings, str)) {
        printf("Nespravny vstup.\n");
        freeWings(wings);
        free(str);
        return 1;
    }

    int sumCount = 0;
    for (int i = 0; i < WING_COUNT; i++) {
        WING* wing = (WING*) wings[i];
        sumCount += wing->count;
    }

    if (counts != sumCount) {
        printf("Nespravny vstup.\n");
        freeWings(wings);
        free(str);
        return 1;
    }

    int searchedIndexes[] = {0,0,0,0};
    TREE* tree = (TREE*) malloc(sizeof(*tree));
    tree->depth = 0;
    tree->index = 0;
    tree->selfBranchIndex = 0;
    tree->value = 0;
    tree->parent = 0;
    tree->wing = 0;
    for (int i = 0; i < WING_COUNT; i++) tree->branches[i] = 0;

    makeTree(tree, wings, searchedIndexes, MAX_REC);
    
    int A = 0;
    int B = 0;

    while (!isLast(tree)) {
        int bestIndex = findBestBranch(tree, tree->depth);
        tree = (TREE*) tree->branches[bestIndex];
        
        WING* wing = (WING*) tree->wing;
        if (tree->depth % 2) A += wing->items[tree->index];
        else B += wing->items[tree->index];

        freeTree((TREE*) tree->parent, bestIndex);
        printBranch(tree);

        searchedIndexes[bestIndex]++;
        makeTree(tree, wings, searchedIndexes, tree->depth + MAX_REC);
    }

    printf("Celkem A/B: %d/%d\n", A, B);

    freeTree(tree, -1);
    freeWings(wings);
    free(str);
}