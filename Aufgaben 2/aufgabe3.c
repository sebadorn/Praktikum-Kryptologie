#include <stdlib.h>
#include <stdio.h>

#include "../lib/crypto.h"


/**
 * Blum Blum Shub generator.
 * @param int stop Number of numbers to generate.
 */
int bbs( int stop ) {
	int p = 11; // prime
	int q = 19; // prime
	int s = 3; // seed
	int n = p * q;
	int xnew = ( s * s ) % n; // start value or seed
	int xold;
	int i;
	int bit;

	for( i = 1; i < stop; i++ ) {
		xold = xnew;
		bit = xnew % 2;
		printf( "%d ", bit );
		xnew = ( xold * xold ) % n;
	}
}


int main( void ) {
	int m = 259200;
	int a = 7141;
	int b = 54773;
	int xold = 141421;
	int xnew;
	int i, r, stop = 20;

	printf( "# lcg:\n" );
	for( i = 0; i < stop; i++ ) {
		xnew = lcg( xold, a, b, m );
		printf( "lcg(%d, %d, %d, %d) = %d\n", xold, a, b, m, xnew );
		xold = xnew;
	}
	printf( "\n" );

	printf( "# bbs:\n" );
	bbs( stop );
	printf( "\n" );

	return EXIT_SUCCESS;
}