#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <cstdint>

/*
spojovy seznam

posledni prvek bude vzdycky prazdny, 
jenom bude mit nastavenou isLast na true
aby program poznal, ze na next uz nic neni 
*/
typedef struct airplane {
	double x, y;
	char* name;
	bool isLast;
	struct airplane* next;
} AIRPLANE;

int readInput(AIRPLANE* planes) {
	int counter = 0;
	
	while (true) {
		double x, y;
		char* name = (char*) malloc(sizeof(*name) * 200);
		int vs = scanf("%lg,%lg:%199s", &x, &y, name);
		
		if (vs == -1) { // EOF
			free(name); // free name mem
			break; 
		}
		else if (vs != 3) { // wrong input
			free(name);	// free name mem
			return 1; 
		}
		
		/*
		nastaveni hodnot posledniho/predposledniho airplane v seznamu
		nastaveni isLast na false, protoze posledni se zmeni na prespoledni
		*/
		planes->x = x;
		planes->y = y;
		planes->isLast = false;
		planes->name = name;
		
		/*
		allocovani novy pameti
		nastaveni isLast na true
		nastaveni next predpolsedniho na posledni
		nastaveni next posledniho na sama sebe (nevim, zkusil sem to, ze by tomu treba mohl vadit NULL pointer)
		*/
		planes->next = (AIRPLANE*) malloc(sizeof(*planes));
		planes->next->isLast = true;
		planes = planes->next;
		planes->next = planes; 
		
		counter++;
	}
	return counter;
}

/*
metoda uvolni pamet celeho spojoveho seznamu
*/
void destroyAirplanes(AIRPLANE* iterator) {
	while (!iterator->isLast) { // cyklus az dokud iterator neni posledni
		AIRPLANE* add = iterator->next; // dalsi v rade
		free(iterator->name); // uvolneni char pameti
		free(iterator); // uvolneni samotneho iterator
		iterator = add; // posun na dalsi
	}
	free(iterator); // uvolneni posledniho prvku
}

bool eqvDouble(double d1, double d2) {
	return fabs(d1 - d2) <= 50 * FLT_EPSILON * ( fabs(d1) + fabs(d2) );
}

double distance(AIRPLANE* i1, AIRPLANE* i2) {
	double vx = i1->x - i2->x;
	double vy = i1->y - i2->y;
	return sqrt(vx*vx + vy*vy);
}

/*
metoda nalezne nejmensi vzdalenost mezi letadly
*/
double findMinDist(AIRPLANE* iterator) {
	double minValue = DBL_MAX;
	
	while (!iterator->isLast) { // od prvniho do predposledniho prvku
		AIRPLANE* iterator2 = iterator->next;
		while (!iterator2->isLast) { // od dalsiho od iterator do predposledniho prvku
			double dist = distance(iterator, iterator2);
			if (dist < minValue) minValue = dist;
			iterator2 = iterator2->next;
		}
		iterator = iterator->next;
	}
	
	return minValue;
}

/*
metoda spocita kolik dvojic, ktere jsou od sebe
minimalni vzdalenost, cely spojak obsahuje
*/
int findDoublesCount(AIRPLANE* iterator, double minDist) {		
	int counter = 0;
	
	while (!iterator->isLast) {
		AIRPLANE* iterator2 = iterator->next;
		while (!iterator2->isLast) {
			double dist = distance(iterator, iterator2);
			if (eqvDouble(minDist, dist)) counter++;
			iterator2 = iterator2->next;
		}
		iterator = iterator->next;
	}
	
	return counter;
}

/*
metoda vypise dvojice, ktere jsou od sebe minimalni vzdalenost
*/
void printDoubles(AIRPLANE* iterator, double minDist) {		
	while (!iterator->isLast) {
		AIRPLANE* iterator2 = iterator->next;
		while (!iterator2->isLast) {
			double dist = distance(iterator, iterator2);
			if (eqvDouble(minDist, dist)) {
				printf("%s - %s\n", iterator->name, iterator2->name);
			}
			iterator2 = iterator2->next;
		}
		iterator = iterator->next;
	}	
}

int main() {
	printf("Pozice letadel:\n");
	
	AIRPLANE* planes = (AIRPLANE*) malloc(sizeof(*planes)); // allokace prvniho prvku
	planes->isLast = true; // nastaveni na true, protoze ted je prazdny a posledni
	planes->next = planes;

	// printf("%u\n", planes); // adresa pred
	int size = readInput(planes); // nacteni inputu
	// printf("%u\n", planes); // adresa po, je stejna jako pred
	if (size <= 1) {
		printf("Nespravny vstup.\n");
		destroyAirplanes(planes);
		return 1;
	}
		
	double minDist = findMinDist(planes);
	printf("Vzdalenost nejblizsich letadel: %lf\n", minDist);
	
	int doubles = findDoublesCount(planes, minDist);
	printf("Nalezenych dvojic: %d\n", doubles);
	printDoubles(planes, minDist);

	destroyAirplanes(planes);
	
	return 0;
}
