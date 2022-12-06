#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>

#define CHAR_BLOCK 10

typedef struct list {
	int count, size, number;
	uintptr_t* items;
	struct list* next;
} LIST;

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

char* readLine() {
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

		if (appendString(string, newStr, &filled)) break;
	}

	free(newStr);
	return string;
}

void makeNewInstance(LIST *list, int number) {
	list->next = (LIST *)malloc(sizeof(*list->next));
	list = list->next;

	list->count = 0;
	list->number = number;
	list->next = NULL;
	if (number == -1) list->size = CHAR_BLOCK;
	else list->size = 1;
	list->items = (uintptr_t*) malloc(sizeof(*list->items) * list->size);
}

void reallocItems(LIST *list) {
	if (list->number == -1 && list->count >= list->size) {
		list->size *= 2;
		list->items = (uintptr_t *)realloc(list->items, sizeof(*list->items) * list->size);
	}
}

int loadLines(LIST *list, bool regal) {
	int iter = 0;
	int nm = 0;

	while (true) {
		char *line = readLine();

		if (!line) return regal; // regals and lists
		else if (strlen(line) == 0) {
			free(line);
			if (regal) break; // regals
			makeNewInstance(list, -1); // lists
			list = list->next;
		}
		else if (line[0] == '#' && regal) { // regals
			int len = strlen(line);

			char* num = (char*) malloc(len);
			for (int i = 0; i < len; i++) {
				char c = line[i + 1];
				if (!isdigit(c) && c != 0) {
					free(line);
					return 1;
				}
				num[i] = c;
			}
			nm = atoi(num);
			free(num);
			free(line);

			if (nm != iter++) return 1;
		}
		else if (iter == 0 && regal) return 1;
		else {
			if (regal) {
				makeNewInstance(list, nm);
				list = list->next;
			}
			list->items[list->count++] = (uintptr_t) line;
			reallocItems(list);
		}
	}
	return 0;
}

int readInput(LIST *regals, LIST *lists) {
	int r = loadLines(regals, true);
	if (r == 1) return 1;

	r = loadLines(lists, false);
	if (r == 1) return 1;

	return 0;
}

LIST *swapFirst(LIST *list) {
	if (!list->next) return list;

	LIST *first = list->next;
	free(list);
	return first;
}

void printList(LIST *list) {
	while (list) {
		if (list->number >= 0) printf("#%d ", list->number);
		else printf("\n");

		for (int i = 0; i < list->count; i++) {
			printf("%s\n", (char *)list->items[i]);
		}

		list = list->next;
	}
}

void freeList(LIST *list) {
	while (list) {
		LIST *temp = list->next;

		if (list->items) {
			for (int i = 0; i < list->count; i++) {
				free((char *)list->items[i]);
			}
			free(list->items);
		}

		free(list);

		list = temp;
	}
}

LIST *initList() {
	LIST *list = (LIST *)malloc(sizeof(*list));
	list->count = 0;
	list->next = NULL;
	list->items = NULL;
	return list;
}

// LOGIC

bool compare(char* s1, char* s2, int len) {
	while (len--) {
    	if (toupper(*s1) != toupper(*s2)) return false;
    	else {
    		s1++;
    		s2++;
    	}
	}
  return true;
}

bool isSubstring(char *str, char *sub, int patternLen) {
	int strLen = strlen(str);
	if (strLen <= patternLen) return false;

	while (*str) {
		if (compare(str, sub, patternLen)) return true;
		str++;
	}
	return false;
}

void optimalize(LIST* list, LIST* regals) {
	LIST* first = regals;
	int regalCount = 0;
	while (regals) {
		regalCount++;
		regals = regals->next;
	}

	int* matches = (int*) malloc(sizeof(matches) * list->count);
	for (int i = 0; i < list->count; i++) matches[i] = regalCount;

	uintptr_t* match = (uintptr_t*) malloc(sizeof(match) * list->count);
	for (int i = 0; i < list->count; i++) match[i] = 0;

	printf("Optimalizovany seznam:\n");

	for (int i = 0; i < list->count; i++) {	
		char* lItem = (char*) list->items[i];
		int lenItem = strlen(lItem);

		regals = first;
		while (regals) {
			if (!regals->count) {
				regals = regals->next;
				continue;
			}
			char* rItem = (char*) regals->items[0];

			int rLenItem = strlen(rItem);
			if (rLenItem != lenItem) {
				regals = regals->next;
				continue;
			}

			if (compare(rItem, lItem, lenItem)) {
				matches[i] = regals->number;
				match[i] = regals->items[0];
				break;
			}
			regals = regals->next;
		}
	}

	for (int i = 0; i < list->count; i++) {	
		if (matches[i] != regalCount) continue;
		char* lItem = (char*) list->items[i];
		int lLen = strlen(lItem);
			
		regals = first;
		while (regals) {
			if (regals->count == 0) {
				regals = regals->next;
				continue;
			}
			char* rItem = (char*) regals->items[0];
			
			if (isSubstring(rItem, lItem, lLen)) {
				matches[i] = regals->number;
				match[i] = regals->items[0];
				break;
			}
			regals = regals->next;
		}
	}

	int c = 0;
	for (int i = 0; i <= regalCount; i++) {
		for (int o = 0; o < list->count; o++) {
			if (i == matches[o]) {
				char* lItem = (char*) list->items[o];
				printf(" %d. %s -> ", c++, lItem);

				if (i == regalCount) printf("N/A\n");
				else {
					char* mat = (char*) match[o];
					printf("#%d %s\n", matches[o], mat);
				}
			}
		}
	}

	free(matches);
	free(match);
}

// #0\n$(%10,100s\n)1000,10000$#1\n$(%10,100s\n)1000,10000$#2\n$(%10,100s\n)1000,10000$#3\n$(%10,100s\n)1000,10000$#4\n$(%10,100s\n)1000,10000$\n$(%100,1000s\n)100,1000$a
int main(int argc, char *argv[]) {
	LIST* regals = initList();

	LIST* lists = initList();
	lists->number = -1;
	lists->size = CHAR_BLOCK;
	lists->items = (uintptr_t *)malloc(sizeof(*lists->items) * lists->size);

	int i = readInput(regals, lists);
	if (i) {
		printf("Nespravny vstup.\n");
		freeList(regals);
		freeList(lists);
		return i;
	}

	regals = swapFirst(regals);
	LIST* firstLists = lists;

	while (lists) {
		optimalize(lists, regals);
		lists = lists->next;
	}

	freeList(regals);
	freeList(firstLists);
}
