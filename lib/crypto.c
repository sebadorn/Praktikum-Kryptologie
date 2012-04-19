#include "crypto.h"

typedef struct {
	int n;
	int x;
	int y;
} gdc_return;


/**
 * ggT
 * @param int a
 * @param int b
 * @return int
 */
int ggT( int a, int b ) {
	if( b == 0 ) {
		return a;
	}
	return ggT( b, a % b );
}


/**
 * kgV
 * @param int a
 * @param int b
 * @return int
 */
int kgV( int a, int b ) {
	return a * b / ggT( a, b );
}


/**
 *
 * @param int a
 * @param int n
 * @return gdc_return
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