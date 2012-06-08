#include <stdio.h>
#include <stdlib.h>

#include "../lib/crypto.h"


int main( void ) {
	int i, j, k, mod = 31;
	long long det;
	/*long long **a = malloc( 3 * sizeof( long long ) );
	double **a_inv = malloc( 3 * sizeof( double ) );
	double **e = malloc( 3 * sizeof( double ) );*/
	long long a[3][3];
	double a_inv[3][3];
	double e[3][3];

	// Arrays suck in C
	/*for( i = 0; i < 3; i++ ) {
		a[i] = malloc( 3 * sizeof( long long ) );
		a_inv[i] = malloc( 3 * sizeof( double ) );
		e[i] = malloc( 3 * sizeof( double ) );
	}*/
/*
	a[0][0] = 1; a[0][1] = 2; a[0][2] = 0;
	a[1][0] = -1; a[1][1] = 1; a[1][2] = 1;
	a[2][0] = 1; a[2][1] = 2; a[2][2] = 3;
	// det(A) = 9
*/
	a[0][0] = 5; a[0][1] = 21; a[0][2] = 4;
	a[1][0] = 1; a[1][1] = 1;  a[1][2] = 7;
	a[2][0] = 2; a[2][1] = 22; a[2][2] = 9;

	// Matrix before inversion
	printf( "----- ----- \n" );
	printf( "Matrix A:\n" );
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			printf( "% 3lld ", a[i][j] );
		}
		printf( "\n" );
	}

	// Determinant
	det = matrix_det( a, 3, mod );
	printf( "----- ----- \n" );
	printf( "det(A) %% %d = %lld\n", mod, det );
	if( mod != 0 ) {
		printf( "ModInv( det(A), %d ) = %lld\n", mod, ModInvS12( det, mod ) );
	}

	// Invert matrix
	matrix_inv( a_inv, a, 3, mod );

	// Matrix after inversion
	printf( "----- ----- \n" );
	printf( "Inverted A:\n" );
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			printf( "% 7.2f ", a_inv[i][j] );
		}
		printf( "\n" );
	}

	// A * A^-1 = E
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			for( k = 0; k < 3; k++ ) {
				e[i][j] += a[i][k] * a_inv[k][j];
			}
			if( mod != 0 ) {
				e[i][j] = ModS12( (long long) e[i][j], mod );
			}
		}
	}

	printf( "----- ----- \n" );
	printf( "A * A^-1 = E\n" );
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			printf( "% 7.2f ", e[i][j] );
		}
		printf( "\n" );
	}

	return EXIT_SUCCESS;
}