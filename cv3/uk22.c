#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int weekDay(int y, int m, int d) {
	int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	if (m < 3) {
		y--;
	}
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

bool isTrans(int y) {
	if (y % 4000 == 0) return false;
	else if (y % 400 == 0) return true;
	else if (y % 100 == 0) return false;
	else if (y % 4 == 0) return true;
	return false;
}

int monthLength(int y, int m) {
	if (m == 2) {
		if (isTrans(y)) return 29;
		else return 28;
	}
	return 30 + (m % 2);
}

int yearLength(int y) {
	if (isTrans(y)) return 365;
	return 364;
}

bool validation(int y, int m, int d, int h, int i) {
	return (y >= 1600 &&
		1 <= m && m <= 12 &&
		1 <= d && d <= monthLength(y, m) &&
		0 <= h && h <= 23 &&
		0 <= i && i <= 59);		
}

int bells ( int y1, int m1, int d1, int h1, int i1,
            int y2, int m2, int d2, int h2, int i2,
            long long int * b1, long long int * b2 )
{
	if (
	!validation(y1, m1, d1, h1, i1) ||
	!validation(y2, m2, d2, h2, i2) ||
	(y1 < y2 && m1 < m2 && d1 < d2 && h1 < h2 && i1 < i2)
	) {
		return 0;		
	}
	
	//int startDay = weekDay(y1, m1, d1);
	//int endDay = weekDay(y2, m2, d2);
	
	long long int bell1 = 0;
	long long int bell2 = 0;
	
	i1 = (i1 / 15) * 15;	
	i2 = (i2 / 15) * 15;
	
	while (true) {
		int ding1 = i1 / 15;
		if (ding1 == 0) ding1 = 4;
		//printf("%d:%d %d\n", h1, i1, ding1);
		bell1 += ding1;
		
		i1 += 15;
		if (i1 >= 60) {
			i1 = 0;
			
			int ding2 = ++h1 % 12;
			if (ding2 == 0) ding2 = 12;
			//printf("%d %d\n", h1, ding2);
			bell2 += ding2;
			
			if (h1 >= 24) {
				h1 = 0;
				d1++;
				
				if (weekDay(y1, m1, d1) == 0) d1++;
				
				int mL = monthLength(y1, m1);
				if (d1 > mL) {
					d1 -= mL;
					m1++;
					if (m1 > 12) {
						m1 = 1;
						y1++;
					}
				}
			}
		}
		if (y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2 && i1 == i2) {
			ding1 = i1 / 15;
			if (ding1 == 0) ding1 = 4;
			//printf("%d:%d %d\n", h1, i1, ding1);
			bell1 += ding1;
			break;
		}
	}
	
	//printf("\n%d %d\n", bell1, bell2); 
	*b1 = bell1;
	*b2 = bell2;
	
	return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int b1, b2;

  assert ( bells ( 2022, 10,  1, 13, 15,
                   2022, 10,  1, 18, 45, &b1, &b2 ) == 1
           && b1 == 56
           && b2 == 20 );
  assert ( bells ( 2022, 10,  3, 13, 15,
                   2022, 10,  4, 11, 20, &b1, &b2 ) == 1
           && b1 == 221
           && b2 == 143 );
  assert ( bells ( 2022, 10,  1, 13, 15,
                   2022, 10,  2, 11, 20, &b1, &b2 ) == 1
           && b1 == 106
           && b2 == 65 );
  assert ( bells ( 2022, 10,  2, 13, 15,
                   2022, 10,  3, 11, 20, &b1, &b2 ) == 1
           && b1 == 115
           && b2 == 78 );
  assert ( bells ( 2022, 10,  1, 13, 15,
                   2022, 10,  3, 11, 20, &b1, &b2 ) == 1
           && b1 == 221
           && b2 == 143 );
  assert ( bells ( 2022,  1,  1, 13, 15,
                   2022, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 56861
           && b2 == 36959 );
  assert ( bells ( 2019,  1,  1, 13, 15,
                   2019, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 57101
           && b2 == 37115 );
  assert ( bells ( 2024,  1,  1, 13, 15,
                   2024, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 57341
           && b2 == 37271 );
  assert ( bells ( 1900,  1,  1, 13, 15,
                   1900, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 57101
           && b2 == 37115 );
  assert ( bells ( 2022, 10,  1,  0,  0,
                   2022, 10,  1, 12,  0, &b1, &b2 ) == 1
           && b1 == 124
           && b2 == 90 );
  assert ( bells ( 2022, 10,  1,  0, 15,
                   2022, 10,  1,  0, 25, &b1, &b2 ) == 1
           && b1 == 1
           && b2 == 0 );
  assert ( bells ( 2022, 10,  1, 12,  0,
                   2022, 10,  1, 12,  0, &b1, &b2 ) == 1
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2022, 11,  1, 12,  0,
                   2022, 10,  1, 12,  0, &b1, &b2 ) == 0
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2022, 10, 32, 12,  0,
                   2022, 11, 10, 12,  0, &b1, &b2 ) == 0
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2100,  2, 29, 12,  0,
                   2100,  2, 29, 12,  0, &b1, &b2 ) == 0
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2000,  2, 29, 12,  0,
                   2000,  2, 29, 12,  0, &b1, &b2 ) == 1
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2004,  2, 29, 12,  0,
                   2004,  2, 29, 12,  0, &b1, &b2 ) == 1
           && b1 == 0
           && b2 == 0 );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
