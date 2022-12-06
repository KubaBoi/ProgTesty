#include <stdio.h>
#include <math.h>
#include <cstdlib>

int readInput(int* arr, int* size) {
	int iterator = 0;
	
	while (true) {
		int a;
		int vs = scanf("%d", &a);
		
		if (vs == -1) break; // end of file 
		else if (vs != 1 || iterator >= 2000) { // wrong input
			return 1;
		}
		arr[iterator++] = a;
	}
	*size = iterator;
	return 0;
}

void printArray(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int isIn(int* arr, int endIndex, int value) {
	for (int i = 0; i <= endIndex; i++) {
		if (arr[i] == value) return i;
	}
	return -1;
}

void nullArray(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = 0;
	}

}

void minMax(int* arr, int size, int* min, int* max) {
	int mn = 0;
	int mx = 0;
	for (int i = 0; i < size-1; i++) {
		int sum = arr[i];
		for (int j = i + 1; j < size; j++) {
			sum += arr[j];
			if (sum < mn) mn = sum;
			if (sum > mx) mx = sum;
		}
	}
	*min = mn;
	*max = mx;
}

void calcIntervals(int* arr, int size) {
	int min, max;
	minMax(arr, size, &min, &max);
	
	int newSize = max - min + 1;
	int* occs = (int*) malloc(sizeof(int) * newSize);
	
	nullArray(occs, newSize);
		
	for (int i = 0; i < size-1; i++) {
		int sum = arr[i];
		for (int j = i + 1; j < size; j++) {
			sum += arr[j];
			
			occs[sum - min]++;
		}
	}
	
	int doublesCount = 0;
	for (int i = 0; i < newSize; i++) {
		doublesCount += (occs[i] * (occs[i] - 1)) / 2;
	}
	
	printf("Pocet dvojic: %d\n", doublesCount);
	free(occs);
} 

int main() {
	printf("Posloupnost:\n");
	
	int* arr = (int*) malloc(sizeof(int) * 2000);
	int size;
	
	if (readInput(arr, &size) == 1 || size == 0) {
		printf("Nespravny vstup.\n");
		free(arr);
		return 1;
	}
	
	calcIntervals(arr, size);
	free(arr);
	
	return 0;
}
