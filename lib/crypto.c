#include "crypto.h"


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
	return ggT( b, modulo( a, b ) );
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
 * Modular inverse.
 * @param int a
 * @param int n
 * @return int
 */
int ModInv( int a, int n ) {
	int i;

	for( i = 1; i < n - 1; i++ ) {
		if( modulo( i * a, n ) == 1 ) {
			return i;
		}
	}
	return 0;
}


/**
 * Calculate determinant of a 2x2 matrix.
 * @param const int a[2][2]
 * @return int Determinant of matrix.
 */
int matrix_det_2x2( const int a[2][2] ) {
	return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}


/**
 * Calculate determinant of a 3x3 matrix.
 * @param const int a[3][3]
 * @return int Determinant of matrix.
 */
int matrix_det_3x3( const int a[3][3] ) {
	int plus = a[0][0] * a[1][1] * a[2][2]
			 + a[0][1] * a[1][2] * a[2][0]
			 + a[0][2] * a[1][0] * a[2][1];
	int minus = a[0][2] * a[1][1] * a[2][0]
			 + a[1][2] * a[2][1] * a[0][0]
			 + a[2][2] * a[0][1] * a[1][0];
	return plus - minus;
}


/**
 * Invert a 2x2 matrix.
 * @param int dest[2][2]
 * @param int src[2][2]
 * @param int modn
 */
void matrix_inv_2x2( int dest[2][2], const int src[2][2], int modn ) {
	int i, j, f;

	f = ModInv( matrix_det_2x2( src ), modn );

	dest[0][0] = f * src[1][1];
	dest[0][1] = -f * src[0][1];
	dest[1][0] = -f * src[1][0];
	dest[1][1] = f * src[0][0];
}


/**
 * Invert a 3x3 matrix.
 * @param int dest[3][3]
 * @param int src[3][3]
 * @param int modn
 */
void matrix_inv_3x3( int dest[3][3], const int src[3][3], int modn ) {
	int i, j, f;

	f = ModInv( matrix_det_3x3( src ), modn );

	dest[0][0] = f * ( src[1][1] * src[2][2] - src[1][2] * src[2][1] );
	dest[0][1] = f * ( src[0][2] * src[2][1] - src[0][1] * src[2][2] );
	dest[0][2] = f * ( src[0][1] * src[1][2] - src[0][2] * src[1][1] );

	dest[1][0] = f * ( src[1][2] * src[2][0] - src[1][0] * src[2][2] );
	dest[1][1] = f * ( src[0][0] * src[2][2] - src[0][2] * src[2][0] );
	dest[1][2] = f * ( src[0][2] * src[1][0] - src[0][0] * src[1][2] );

	dest[2][0] = f * ( src[1][0] * src[2][1] - src[1][1] * src[2][0] );
	dest[2][1] = f * ( src[2][0] * src[0][1] - src[0][0] * src[2][1] );
	dest[2][2] = f * ( src[0][0] * src[1][1] - src[0][1] * src[1][0] );
}


/**
 * Modulo.
 * Spot the difference.
 * Python:	-4 % 17 = 13 (good)
 * C:		-4 % 17 = -4 (bad)
 * @param int a
 * @param int b
 * @return int
 */
int modulo( int a, int b ) {
	while( a < 0 ) {
		a += b;
	}
	return a % b;
}


/**
 * Euler's phi function.
 * @param int n
 * @return int Number of natural numbers smaller than n and coprime to n.
 */
int EulerPhi( int n ) {
	int i, ep = 0;

	for( i = 0; i < n; i++ ) {
		if( ggT( i, n ) == 1 ) {
			ep++;
		}
	}
	return ep;
}