#include <stdio.h>
#include <stdlib.h>

#include "../lib/crypto.h"


unsigned long long eEAdS12( unsigned long long a, unsigned long long b,
		unsigned long long d, long long *x, long long *y ) {
	long long ggT, q, r,
		u = 1, v = 0, s = 0, t = 1;

	while( b > 0 ) {
		q = a / b;
		r = a - q * b; a = b; b = r;
		r = u - q * s; u = s; s = r;
		r = v - q * t; v = t; t = r;
	}
	ggT = a;

	s = d / ggT;
	if( d == 0 ) {
		return 0;
	}
	*x = s * u;
	*y = s * v;

	return ggT;
}


int main( void ) {
	unsigned long long
		a = 45948, b = 32576,
		c = 24877, d = 36491,
		e = 13, f = 19, g = 369;
	long long
		*x = malloc( sizeof( long long ) ),
		*y = malloc( sizeof( long long ) );

	printf( "eEA1S12(%llu, %llu) = %d\n", a, b, eEA1S12( a, b, x, y ) );
	printf( "eEA1S12(%llu, %llu) = %d", c, d, eEA1S12( c, d, x, y ) );
	printf( "\tx: %lld  y: %lld\n", *x, *y );
	printf( "eEAdS12(%llu, %llu, %llu) = %llu", e, f, g, eEAdS12( e, f, g, x, y ) );
	printf( "\tx: %lld  y: %lld\n", *x, *y );

	free( x );
	free( y );
	return EXIT_SUCCESS;
}