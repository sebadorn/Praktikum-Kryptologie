#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/crypto.h"



int main( void ) {
	unsigned long long
		a = 45948, b = 32576,
		c = 24877, d = 36491,
		e = 13, f = 19, g = 369,
		h = 500,
		i = 0, j = 10,
		k = 10,
		l = 24877,
		count, tmp;
	long long
		*x = malloc( sizeof( long long ) ),
		*y = malloc( sizeof( long long ) );
	unsigned long long
		*z;

	// Use new functions
	printf( "eEA1S12(%llu, %llu, x, y) = %llu\n", a, b, eEA1S12( a, b, x, y ) );

	printf( "eEA1S12(%llu, %llu, x, y) = %llu", c, d, eEA1S12( c, d, x, y ) );
	printf( "\tx: %lld  y: %lld\n", *x, *y );

	printf( "eEAdS12(%llu, %llu, %llu, x, y) = %llu", e, f, g, eEAdS12( e, f, g, x, y ) );
	printf( "\tx: %lld  y: %lld\n", *x, *y );

	printf( "prime1S12(%llu) = %llu\n", h, prime1S12( h ) );

	printf( "primeZS12(%llu, %llu) = %llu\n", i, j, primeZS12( i, j ) );

	tmp = primeNS12( &z, k );
	printf( "primeNS12(z, %llu) = %llu\n", k, tmp );
	printf( "\tz: " );
	for( count = 0; count < tmp; count++ ) {
		printf( "%llu ", z[count] );
	}
	printf( "\n" );

	printf( "IsPrimeS12(%llu) = %d\n", l, IsPrimeS12( l ) );

	free( x );
	free( y );
	return EXIT_SUCCESS;
}