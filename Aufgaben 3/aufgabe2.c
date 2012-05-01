#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/crypto.h"


/**
 * Calculates all primes p with 2 <= p <= n.
 * @param unsigned long long *ptr
 * @param unsigned long long n
 * @return unsigned long long Number of primes p <= n.
 */
unsigned long long primeNS12( unsigned long long *ptr, unsigned long long n ) {

}


int main( void ) {
	unsigned long long
		a = 45948, b = 32576,
		c = 24877, d = 36491,
		e = 13, f = 19, g = 369,
		h = 500,
		i = 0, j = 10,
		k = 100,
		l = 24877;
	long long
		*x = malloc( sizeof( long long ) ),
		*y = malloc( sizeof( long long ) );

	printf( "eEA1S12(%llu, %llu) = %llu\n", a, b, eEA1S12( a, b, x, y ) );
	printf( "eEA1S12(%llu, %llu) = %llu", c, d, eEA1S12( c, d, x, y ) );
	printf( "\tx: %lld  y: %lld\n", *x, *y );
	printf( "eEAdS12(%llu, %llu, %llu) = %llu", e, f, g, eEAdS12( e, f, g, x, y ) );
	printf( "\tx: %lld  y: %lld\n", *x, *y );
	printf( "prime1S12(%llu) = %llu\n", h, prime1S12( h ) );
	printf( "primeZS12(%llu, %llu) = %llu\n", i, j, primeZS12( i, j ) );
	printf( "IsPrimeS12(%llu) = %d\n", l, IsPrimeS12( l ) );

	free( x );
	free( y );
	return EXIT_SUCCESS;
}