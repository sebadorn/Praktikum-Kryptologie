#include <stdlib.h>
#include <stdio.h>

#include "../lib/crypto.h"


void MultGroup( unsigned long long n ) {
	unsigned long long i;

	for( i = 1; i < n; i++ ) {
		if( ggTS12( i, n ) == 1 ) {
			printf( "%llu ", i );
		}
	}
}


int main( void ) {
	unsigned long long
		n = 36,
		t = EulPhiS12( n );

	printf( "EulPhiS12(%llu) = %llu\n", n, t );
	printf( "MultGroup(%llu): ", n );
	MultGroup( n );
	printf( "\n" );

	return EXIT_SUCCESS;
}