#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../lib/crypto.h"

long long KEY[3][3]; // "HILLKEY!"
int MODN = 0; // 31


/**
 * Special version of matrix_det() for this exercise.
 */
long long matrix_det_a62( long long a[3][3], unsigned int n, int mod ) {
	int i, j, k, l;
	long long det;
	long long **m = NULL;

	if( n == 1 ) { det = a[0][0]; }
	else if( n == 2 ) { det = a[0][0] * a[1][1] - a[1][0] * a[0][1]; }
	else {
		det = 0;
		for( i = 0; i < n; i++ ) {
			m = malloc( ( n - 1 ) * sizeof( long long *) );
			for( j = 0; j < n - 1; j++ ) {
				m[j] = malloc( ( n - 1 ) * sizeof( long long ) );
			}

			for( j = 1; j < n; j++ ) {
				k = 0;
				for( l = 0; l < n; l++ ) {
					if( l == i ) { continue; }
					m[j - 1][k++] = a[j][l];
				}
			}

			det += pow( -1.0, 2.0 + i ) * a[0][i] * matrix_det( m, n - 1, 0 );

			for( j = 0; j < n - 1; j++ ) { free( m[j] ); }
			free( m );
		}
	}

	return mod == 0 ? det : det % MODN;
}


/**
 * Special version of matrix_adj() for this exercise.
 */
void matrix_adj_a62( long long dest[3][3], long long a[3][3], unsigned int n ) {
	int i, j, k, l, m, o;
	long long det, tmp;
	long long **c = NULL;

	c = malloc( ( n - 1 ) * sizeof( long long *) );
	for( i = 0; i < n - 1; i++ ) {
		c[i] = malloc( ( n - 1 ) * sizeof( long long ) );
	}

	for( i = 0; i < n; i++ ) {
		for( j = 0; j < n; j++ ) {

			k = 0;
			for( l = 0; l < n; l++ ) {
				if( l == j ) { continue; }
				m = 0;
				for( o = 0; o < n; o++ ) {
					if( o == i ) { continue; }
					c[k][m++] = a[l][o];
				}
				k++;
			}

			det = matrix_det( c, n - 1, MODN );
			dest[j][i] = pow( -1.0, j + i + 2.0 ) * det;
		}
	}

	for( i = 0; i < n - 1; i++ ) { free( c[i] ); }
	free( c );

	// Transpose
	for( i = 1; i < n; i++ ) {
		for( j = 0; j < i; j++ ) {
			tmp = dest[i][j];
			dest[i][j] = dest[j][i];
			dest[j][i] = tmp;
		}
	}
}


/**
 * Special version of matrix_inv() for this exercise.
 */
void matrix_inv_a62( long long dest[3][3], long long a[3][3] ) {
	unsigned int i, j;
	long long det = matrix_det_a62( a, 3, MODN );
	int det_inv = det % MODN;

	if( det == 0 ) {
		printf( "NOTE: No inverse of matrix possible: Determinant equals 0.\n" );
		return;
	}

	matrix_adj_a62( dest, a, 3 );
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			dest[i][j] = ( dest[i][j] * det_inv ) % MODN;
		}
	}
}


/**
 * Multiply two matrices (or a matrix and a vector).
 * @param long long *dest Matrix with the result.
 * @param long long *a Matrix a.
 * @param long long *b Matrix b.
 */
void matrix_mult( long long dest[3][3], long long a[3][3], long long b[3][3] ) {
	int i, j, k = 0;

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			dest[i][j] = 0;
			for( k = 0; k < 3; k++ ) {
				dest[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}


/**
 *
 * @param long long (*dest)[9]
 * @param char *src
 * @param int blocks
 */
void msg_to_blocks( long long (*dest)[9], char *src, int blocks ) {
	int i, j, k = 0;
	long long sum = 0;

	for( i = 0; i < blocks; i++ ) {
		for( j = 0; j < 9; j++ ) {
			// Parity
			if( i == 1 && j == 1 ) { continue; }

			dest[i][j] = ( k < strlen( src ) ) ? (int) src[k++] - 'A' : 0;
			sum += dest[i][j];
		}
		dest[1][1] = sum % MODN;
		sum = 0;
	}
}


/**
 *
 * @param char *src
 * @return int
 */
int count_blocks_needed( char *src ) {
	int blocks = strlen( src ) / 8;

	return strlen( src ) % 8 != 0 ? blocks + 1 : blocks;
}


/**
 *
 * @param char *enc
 * @param char *plain
 */
void hillverH33( char *enc, char *plain ) {
	int i, j, k = 0, t;
	int blocks_needed = count_blocks_needed( plain );
	long long m_enc[3][3];
	long long msg_blocks[blocks_needed][9];
	long long mb[3][3] = {{0},{0},{0}};

	msg_to_blocks( msg_blocks, plain, blocks_needed );

	for( i = 0; i < blocks_needed; i++ ) {
		for( t = 0; t < 9; t++ ) {
			(*mb)[t] = msg_blocks[i][t];
		}
		matrix_mult( m_enc, mb, KEY );

		t = 0;
		for( j = 0; j < 3; j++ ) {
			for( k = 0; k < 3; k++ ) {
				m_enc[j][k] = modulo( m_enc[j][k], MODN );
				enc[t++] = m_enc[j][k] + 'A';
			}
		}
	}
	enc[t] = '\0';
}


/**
 *
 * @param char *decr
 * @param char *encrypted
 */
void hillentH33( char *decr, char *encrypted ) {
	int i, j, k = 0, t;
	int blocks_needed = count_blocks_needed( encrypted );
	long long inv_key[3][3];
	long long m_decr[3][3];
	long long msg_blocks[blocks_needed][9];
	long long mb[3][3] = {{0},{0},{0}};

	msg_to_blocks( msg_blocks, encrypted, blocks_needed );
	matrix_inv_a62( inv_key, KEY );

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			inv_key[i][j] = modulo( inv_key[i][j], MODN );
		}
	}

	for( i = 0; i < blocks_needed; i++ ) {
		for( t = 0; t < 9; t++ ) {
			(*mb)[t] = msg_blocks[i][t];
		}
		matrix_mult( m_decr, mb, inv_key );

		t = 0;
		for( j = 0; j < 3; j++ ) {
			for( k = 0; k < 3; k++ ) {
				m_decr[j][k] = modulo( m_decr[j][k], MODN );
				decr[t++] = m_decr[j][k] + 'A';
			}
		}
	}
	decr[t] = '\0';
}


int main( int argc, char *argv[] ) {
	char *msg; // "lestersh"
	char *ciph;
	int i, j = 0, k = 0;
	long long sum = 0, key_det;

	if( argc < 4 ) {
		printf( "  ERROR: Not enough parameters.\n" );
		return EXIT_FAILURE;
	}

	msg = argv[2];
	ciph = malloc( sizeof( char ) * strlen( msg ) );

	MODN = atoi( argv[3] );

	// Convert key from string to matrix.
	for( i = 0; i < strlen( argv[1] ); i++ ) {
		// Skip parity
		if( j == 1 && k == 1 ) { k++; }

		KEY[j][k++] = argv[1][i];
		sum += argv[1][i];

		if( k >= 3 ) {
			k = 0;
			j++;
		}
	}
	// Parity
	KEY[1][1] = sum % MODN;

	// For decryption, gcd( det KEY, MODN ) has to equal 1.
	key_det = matrix_det_a62( KEY, 3, MODN );
	printf( "  ggT( det K, n ) == %llu\n", ggTS12( key_det, MODN ) );

	// Encrypt
	hillverH33( ciph, msg );
	printf( "  Cipher: %s\n", ciph );

	// Decrypt
	hillentH33( msg, ciph );
	printf( "  Decrypted: %s\n", msg );

	// Clean up
	free( ciph );

	return EXIT_SUCCESS;
}