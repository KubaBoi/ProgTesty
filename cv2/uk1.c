#include <stdio.h>
#include <math.h>
#include <cstdlib>

int readInp(double* arr, int index, char name) {
	double x, y;

	printf("Bod %c:\n", name);
	if (scanf("%lf %lf", &x, &y) != 2) {
		printf("Nespravny vstup.\n");
		return -1;	
	}
	
	arr[index++] = x;
	arr[index++] = y;
	return index;
}

bool doubleEqual(double a, double b) {
	return (abs(a - b) <= 0.00001);
}

bool areTheSame(double* arr, int size) {
	for (int i = 0; i < size; i += 2) {
		for (int j = 0; j < size; j += 2) {
			if (i == j) continue;
			
			if (doubleEqual(arr[i], arr[j]) && doubleEqual(arr[i+1], arr[j+1])) return true;
		}
	}
	return false;
}

bool areInLine(double* arr) {
	double* vectors = (double*) malloc(4 * sizeof(double));
	double* oneVectors = (double*) malloc(4 * sizeof(double));
	double* sizes = (double*) malloc(2 * sizeof(double));
	
	double oldX = 0;
	double oldY = 0;
	
	for (int i = 0; i < 2; i++) {
		int x = i * 2;
		int y = i * 2 + 1;
		
		vectors[x] = arr[0] - arr[(i + 1) * 2];
		vectors[y] = arr[1] - arr[(i + 1) * 2 + 1];
		
		double vsize = sqrt((vectors[x] * vectors[x]) + (vectors[y] * vectors[y]));
		sizes[i] = vsize;		
		//printf("size %d (%d, %d): %lf\n", i, x, y, vsize); 
		
		oneVectors[x] = vectors[x] / vsize;
		oneVectors[y] = vectors[y] / vsize;
		
		double newX = abs(oneVectors[x]);
		double newY = abs(oneVectors[y]);
		
		if (i > 0) {
			if (doubleEqual(newX, oldX) && doubleEqual(newY, oldY)) {
				printf("Body lezi na jedne primce.\n");
			}
			else {
				printf("Body nelezi na jedne primce.\n");
				return false;
			}
		}
		
		oldX = newX;
		oldY = newY;
	}
	
	//printf("vectors: %lf %lf %lf %lf\n", vectors[0], vectors[1], vectors[2], vectors[3]);
	//printf("one vectors: %lf %lf %lf %lf\n", oneVectors[0], oneVectors[1], oneVectors[2], oneVectors[3]);
	
	bool dE1 = doubleEqual(oneVectors[0], oneVectors[2]);
	bool dE2 = doubleEqual(oneVectors[1], oneVectors[3]);
	
	if ((!dE1 && !dE2) || 
		(!dE1 && doubleEqual(oneVectors[1], 0.0) && doubleEqual(oneVectors[3], 0.0)) ||
		(!dE2 && doubleEqual(oneVectors[0], 0.0) && doubleEqual(oneVectors[2], 0.0))) {
		printf("Prostredni je bod A.\n");
	}
	else if (sizes[0] < sizes[1]) {
		printf("Prostredni je bod B.\n");
	}
	else {
		printf("Prostredni je bod C.\n");
	}
	
	return true;
}

int main() {
	int index = 0;
	double* arr = (double*) malloc(6 * sizeof(double));
	char names[] = {'A', 'B', 'C'};
	
	for (int i = 0; i < 3; i++) {
		index = readInp(arr, index, names[i]);
		if (index == -1) return 1;
	}
	
	//printf("points: %lf %lf %lf %lf %lf %lf\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
	
	if (areTheSame(arr, 6)) printf("Nektere body splyvaji.\n");
	else areInLine(arr);
}
