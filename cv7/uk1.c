#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>

#define CHAR_BLOCK 10
#define WING_COUNT 4    

typedef struct wing {
    char name;
    int size, count;
    int* items;
} WING;

typedef struct tree {
    char name;
    int index, value, valueA, valueB;
    unsigned int depth;
    uintptr_t parent;
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

char* readInput() {
    char* str = readLines();
    if (str) removeWhiteSpaces(str);
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

TREE* makeTree(uintptr_t parent, uintptr_t* wings, unsigned int depth, WING* wing, int valueA, int valueB, int ind, int* searchedIndexes) {
    TREE* tree = (TREE*) malloc(sizeof(*tree));
    tree->parent = parent;
    tree->depth = depth;
    tree->valueA = valueA;
    tree->valueB = valueB;
    tree->index = ind;
    if (wing) {
        tree->name = wing->name;
        tree->value = wing->items[ind];
    }
    else {
        tree->name = 'O';
        tree->value = 0;
    }
    
    for (int i = 0; i < WING_COUNT; i++) {
        WING* w = (WING*) wings[i];
        int index = searchedIndexes[i];
        if (index < w->count) {
            int vA = valueA;
            int vB = valueB;
            if (depth % 2) vB += w->items[index];
            else vA += w->items[index];
            
            int* sIndxs = (int*) malloc(sizeof(sIndxs) * WING_COUNT);
            for (int j = 0; j < WING_COUNT; j++) sIndxs[j] = searchedIndexes[j];

            sIndxs[i]++;
            tree->branches[i] = (uintptr_t) makeTree((uintptr_t) tree, wings, depth+1, w, vA, vB, index, sIndxs);
            free(sIndxs);
        }
        else tree->branches[i] = 0;
    }
    return tree;
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
    printf("%c: %c[%d] (%d)\n", player, tree->name, tree->index, tree->value);
}

void printDepth(TREE* tree, unsigned int depth) {
    if (tree->depth == depth) {
        printBranch(tree);
    }

    for (int i = 0; i < WING_COUNT; i++) {
        if (tree->branches[i]) {
            printDepth((TREE*) tree->branches[i], depth);
        }
    }
}

int getDepthValue(TREE* parent, int* vA, int* vB) {
    if (!parent) return 1;

    int valueA = 0;
    int valueB = 0;
    for (int i = 0; i < WING_COUNT; i++) {
        TREE* branch = (TREE*) parent->branches[i];
        if (branch) {
            valueA += branch->valueA;
            valueB += branch->valueB;
        } 
    }
    *vA = valueA;
    *vB = valueB;
    return 0;
}

TREE* findBestBranch(TREE* tree, TREE* best, unsigned int depth) {    
    if (depth % 2) {
        if (tree->valueA > best->valueA) best = tree;
    }
    else {
        if (tree->valueA < best->valueA) best = tree;
    }
    best = tree;

    for (int i = 0; i < WING_COUNT; i++) {
        if (tree->branches[i]) {
            best = findBestBranch((TREE*) tree->branches[i], best, depth);
        }
    }
    return best;
}

TREE* findParent(TREE* tree, unsigned int depth) {
    if (tree->depth == depth) return tree;
    
    TREE* parent = (TREE*) tree->parent;
    while (parent) {
        if (parent->depth == depth) break;
        parent = (TREE*) parent->parent;
    }
    return parent;
}

void freeTree(TREE* tree) {
    for (int i = 0; i < WING_COUNT; i++) {
        if (tree->branches[i]) freeTree((TREE*) tree->branches[i]);
    }
    free(tree);
}

int main() {
    printf("Zetony:\n");
    char* str = readInput();

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

    int searchedIndexes[] = {0,0,0,0};
    TREE* tree = makeTree(0, wings, 0, NULL, 0, 0, 0, searchedIndexes);
    TREE* root = tree;
    TREE* best = NULL;

    unsigned int depth = 1;
    while (true) {
        best = findBestBranch(tree, tree, depth);
        tree = findParent(best, depth++);
        if (!tree) break;
        printBranch(tree);
    }

    printf("Celkem A/B: %d/%d\n", best->valueA, best->valueB);

    freeTree(root);
    freeWings(wings);
    free(str);
}