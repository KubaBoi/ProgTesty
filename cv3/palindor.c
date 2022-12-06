#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#endif /* __PROGTEST__ */

#include <climits>

void toString(unsigned long long value, int radix, char* str, short* size) {
	short counter = 0;
	while (true) {
		short rest = value % radix;
		value = value / radix;
		
		/*if (counter == *size) {
			str = (char*) realloc(str, ++*size);
		}*/
		str[counter] = rest;
		
		//printf("%llu %d %c %s\n", value, rest, str[counter], str); 
		counter++;
		if (value == 0) break;
	}
	*size = counter;
}

bool isPalindrome(char* str, short size) {
	for (short i = 0; i < size; i++) {		
		//printf("%c %c\n", str[i], str[size - i - 1]);
		if (str[i] != str[size - i - 1]) return false;
	}
	return true;
}

int nextPalindrome ( unsigned long long from, 
                     int radix,
                     unsigned long long * next )
{
	//printf("TESTING: %llu %d\n", from, radix);
	if (2 < radix && radix > 36) return 0;
 	if (from >= ULLONG_MAX) return 0;
 	
  	short size = 64;
  	char* str;
  	
  	unsigned long long test = 18446744073709551615LLU;
  
  	while (++from < ULLONG_MAX) {
		str = (char*) malloc(size);
		toString(from, radix, str, &size);
	  	if (isPalindrome(str, size)) {
	  		printf("RETURN %lld %s\n", from, str);
	  		*next = from;
	  		free(str);
	  		return 1;
	  	}
	  	free(str);
	}
	*next = ULLONG_MAX;
	return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  unsigned long long next;

	nextPalindrome(5651651651616, 8, &next);

  assert ( nextPalindrome ( 123, 10, &next ) == 1
           && next == 131 );
  assert ( nextPalindrome ( 188, 10, &next ) == 1
           && next == 191 );
  assert ( nextPalindrome ( 1441, 10, &next ) == 1
           && next == 1551 );
  assert ( nextPalindrome ( 95, 15, &next ) == 1
           && next == 96 ); /* 96 = 66 (radix 15) */
  assert ( nextPalindrome ( 45395, 36, &next ) == 1
           && next == 45431 ); /* 45431 = z1z (radix 36) */
  assert ( nextPalindrome ( 1027, 2, &next ) == 1
           && next == 1057 ); /* 1057 = 10000100001 (radix 2) */
  assert ( nextPalindrome ( 1000, 100, &next ) == 0
           && next == 1057 );
  assert ( nextPalindrome ( 18446744073709551614LLu, 2, &next ) == 1
           && next == 18446744073709551615LLu ); /* 18446744073709551615 = 1111111111111111111111111111111111111111111111111111111111111111 (radix 2) */
  assert ( nextPalindrome ( 18446744073709551615LLu, 2, &next ) == 0
           && next == 18446744073709551615LLu );
  return 0;
}
#endif /* __PROGTEST__ */
