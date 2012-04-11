#include<stdio.h>
#include<stdlib.h>


typedef struct {
	int n;
	int x;
	int y;
} gdc_return;


/**
 *
 * @param int a
 * @param int n
 * @return
 */
gdc_return gdc( int a, int n ) {
	int x = 0;
	int y = 1;
	int u = 1;
	int v = 0;
	double q, r, m, s;
	gdc_return gdc_r;

	while( a != 0 ) {
		q = n / a;
		r = n % a;
		m = x - u * q;
		s = y - v * q;

		n = a; a = r;
		x = u; y = v;
		u = m; v = s;
	}

	gdc_r.n = n;
	gdc_r.x = x;
	gdc_r.y = y;

	return gdc_r;
}


/**
 * Modular inverse.
 * @param int a
 * @param int n
 * @return int
 */
int ModInv( int a, int n ) {
	gdc_return g = gdc( a, n );

	// Spot the difference.
	// Python:	-4 % 17 = 13 (good)
	// C:		-4 % 17 = -4 (bad)
	// Correct that in case g.x is negative.
	while( g.x < 0 ) {
		g.x += n;
	}

	return ( g.n != 1 ) ? 0 : ( g.x % n );
}


int main( void ) {
	int f;
	f = ModInv( 4, 17 );
	printf( "ModInv( 4, 17 ) == %d\n", f );

	return EXIT_SUCCESS;
}