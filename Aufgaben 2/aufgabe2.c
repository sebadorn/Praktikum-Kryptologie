#include <stdlib.h>
#include <stdio.h>

#include "../lib/crypto.h"


void MultGroup( int n ) {
	int i;

	for( i = 1; i < n; i++ ) {
		if( ggT( i, n ) == 1 ) {
			printf( "%d ", i );
		}
	}
}


int main( void ) {
	int n = 36;
	unsigned long long t = EulPhiS12( n );

	printf( "EulPhiS12(%d) = %llu\n", n, t );
	printf( "MultGroup(%d): ", n );
	MultGroup( n );
	printf( "\n" );

	return EXIT_SUCCESS;
}