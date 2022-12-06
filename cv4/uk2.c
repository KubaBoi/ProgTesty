#include <stdio.h>
#include <math.h>
#include <cstdlib>

#define MAX_ARR_SIZE (100 * 100)

int readInput(int* arr, int* size, int* width, int* height) {
	int iterator = 0;
	int vs;
	int wdth = 0;
	int hght = 0;
	bool isDone = false;
	
	while (true) {
		int a;
		int w = 0;
		vs = 1;
		while (vs == 1) {
			vs = scanf("%d", &a);
			int ch = getchar();
			if (vs != 1) {
				if (ch == (int)'\n') vs = -1;
				break;
			}
			arr[iterator++] = a;
			w++;
			
			if (ch == (int)'\n') break;
		}
		if (w > wdth) {
			if (isDone) return 1;
			wdth = w;
			isDone = true;
		}
		
		if (vs == -1) break;
		else if (vs == 0) return 1;
		hght++;
	}
	
	if (wdth * hght != iterator) return 1;
	 
	*width = wdth;
	*height = hght;
	*size = iterator;
	return 0;
}

void printArray(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int getIndex(int w, int h, int x, int y) {
	return (w * y + (x % h));
}

void print2dArray(int* arr, int size, int w, int h) {
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			printf("%d ", arr[getIndex(w, h, x, y)]);
		}
		printf("\n");
	}
}

bool horSym(int* arr, int w, int h) {
	for (int y = 0; y < h / 2; y++) {
		int opY = h - y - 1;
		for (int x = 0; x < w; x++) {
			if (arr[getIndex(w, h, x, y)] != arr[getIndex(w, h, x, opY)]) return false;
		}
	}
	return true;
}

bool vertSym(int* arr, int w, int h) {
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w / 2 + 1; x++) {
			int opX = w - x - 1;
			printf("%d, %d, %d\n", w, x, opX);
			printf("%d %d\n", arr[getIndex(w, h, x, y)], arr[getIndex(w, h, opX, y)]);
			printf("%d\n", getIndex(w, h, opX, y));
			if (arr[getIndex(w, h, x, y)] != arr[getIndex(w, h, opX, y)]) return false;
		}
	}
	return true;
}

bool midSym(int* arr, int size) {
	for (int i = 0; i < size / 2; i++) {
		if (arr[i] != arr[size - i - 1]) return false;
	}
	return true;
}

int main() {
	printf("Matice:\n");
	
	int* arr = (int*) malloc(sizeof(int) * MAX_ARR_SIZE);
	int size, width, height;
	
	if (readInput(arr, &size, &width, &height) == 1 || size == 0) {
		printf("Nespravny vstup.\n");
		free(arr);
		return 1;
	}
	
	printf("Symetrie podle horizontalni osy: ");
	if (horSym(arr, width, height)) printf("ano\n");
	else printf("ne\n");
	
	printf("Symetrie podle vertikalni osy: ");
	if (vertSym(arr, width, height)) printf("ano\n");
	else printf("ne\n");
	
	printf("Symetrie podle stredu: ");
	if (midSym(arr, size)) printf("ano\n");
	else printf("ne\n");
	
	free(arr);
	
	return 0;
}
