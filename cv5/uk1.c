#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <cstdint>

typedef struct airplane {
	double x, y;
	char* name;
	bool isLast;
	struct airplane* next;
} AIRPLANE;

int readInput(AIRPLANE* planes, int* size) {
	int counter = 0;
	
	while (true) {
		double x, y;
		char* name = (char*) malloc(sizeof(*name) * 199);
		int vs = scanf("%lg,%lg:%199s", &x, &y, name);
		
		if (vs == -1) {
			free(name);
			break; 
		}
		else if (vs != 3) {
			free(name);	
			return 1; // wrong input
		}
		
		planes->x = x;
		planes->y = y;
		planes->isLast = false;
		planes->name = name;
		
		planes->next = (AIRPLANE*) malloc(sizeof(*planes->next));
		planes->next->isLast = true;
		planes = planes->next;
		
		counter++;
	}
	*size = counter;
	return 0;
}

void destroyAirplanes(AIRPLANE* iterator) {
	while (!iterator->isLast) {
		uintptr_t add = (uintptr_t) iterator->next;
		free(iterator->name);
		free(iterator);
		iterator = (AIRPLANE*) add;
	}
	free(iterator);
}

bool eqvDouble(double d1, double d2) {
	return fabs(d1 - d2) <= 50 * FLT_EPSILON * ( fabs(d1) + fabs(d2) );
}

void nullArray(uintptr_t* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = 0;
	}
}

double distance(AIRPLANE* i1, AIRPLANE* i2) {
	double vx = i1->x - i2->x;
	double vy = i1->y - i2->y;
	return sqrt(vx*vx + vy*vy);
}

double findMinDist(AIRPLANE* iterator) {
	double minValue = DBL_MAX;
	
	while (!iterator->isLast) {
		AIRPLANE* iterator2 = iterator->next;
		while (!iterator2->isLast) {
			double dist = distance(iterator, iterator2);
			if (dist < minValue) minValue = dist;
			iterator2 = iterator2->next;
		}
		iterator = iterator->next;
	}
	
	return minValue;
}

int findDoubles(AIRPLANE* iterator, uintptr_t* addrs1, uintptr_t* addrs2, double minDist) {		
	int counter = 0;
	
	while (!iterator->isLast) {
		AIRPLANE* iterator2 = iterator->next;
		while (!iterator2->isLast) {
			double dist = distance(iterator, iterator2);
			if (eqvDouble(minDist, dist)) {
				addrs1[counter] = (uintptr_t) iterator;
				addrs2[counter++] = (uintptr_t) iterator2;
			}
			iterator2 = iterator2->next;
		}
		iterator = iterator->next;
	}
	
	return counter;
}

int main() {
	printf("Pozice letadel:\n");
	
	int size;
	AIRPLANE* planes = (AIRPLANE*) malloc(sizeof(*planes));
	planes->isLast = true;
	
	if (readInput(planes, &size) == 1 || size <= 1) {
		printf("Nespravny vstup.\n");
		destroyAirplanes(planes);
		return 1;
	}
		
	double minDist = findMinDist(planes);
	printf("Vzdalenost nejblizsich letadel: %lf\n", minDist);
	
	uintptr_t* addrs1 = (uintptr_t*) malloc(sizeof(*addrs1) * size * size);
	uintptr_t* addrs2 = (uintptr_t*) malloc(sizeof(*addrs2) * size * size);
	
	int doubles = findDoubles(planes, addrs1, addrs2, minDist);
	printf("Nalezenych dvojic: %d\n", doubles);
	for (int i = 0; i < doubles; i++) {
		AIRPLANE* plane1 = (AIRPLANE*) addrs1[i];
		AIRPLANE* plane2 = (AIRPLANE*) addrs2[i];
		printf("%s - %s\n", plane1->name, plane2->name);
	}

	free(addrs1);
	free(addrs2);
	destroyAirplanes(planes);
}
