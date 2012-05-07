#include <stdio.h>
#include <stdlib.h>

#include "../lib/crypto.h"



/**
 *
 * @param long long **a Matrix.
 * @param unsigned int n Length of matrix
 */
void matrix_inv( long long **a, unsigned int n ) {
	//
}


int main( void ) {
	int i, j;
	long long det;
	long long **a = malloc( 3 * sizeof( long long ) );

	// Arrays suck in C
	for( i = 0; i < 3; i++ ) {
		a[i] = malloc( 3 * sizeof( long long ) );
		for( j = 0; j < 3; j++ ) {
			a[i][j] = ( i + 1 ) * 10 + j + 1;
		}
	}

	det = matrix_det( a, 3 );
	printf( "det(a) = %llu\n", det );

	// Clean up
	for( i = 0; i < 3; i++ ) {
		free( a[i] );
	}
	free( a );

	return EXIT_SUCCESS;
}