#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/crypto.h"

#define MODN 27
#define DIM 2

const char SECRET_TEXT[] = "UBIXGTENERHLNHAHRM";
//const char SECRET_TEXT[] = "HELP"; // Wikipedia example
const int KEY[DIM][DIM] = {
	{ 0, 1 },
	{ 7, 15 }
};
//const int KEY[DIM][DIM] = { 3, 3, 2, 5 }; // Wikipedia example



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
void matrix_modinv_2x2( int dest[2][2], const int src[2][2], int modn ) {
	int f = ModInv( matrix_det_2x2( src ), modn );

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
void matrix_modinv_3x3( int dest[3][3], const int src[3][3], int modn ) {
	int f = ModInv( matrix_det_3x3( src ), modn );

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
 * Multiply two matrices (or a matrix and a vector).
 * @param int *dest Matrix with the result.
 * @param int *a Matrix a.
 * @param int *b Matrix b.
 * @param int rows Number of rows in matrix.
 * @param int cols Number of columns in matrix.
 */
void matrix_mult( int *dest, int *a, const int b[DIM][DIM], int rows, int cols ) {
	int i, j;

	for( i = 0; i < rows; i++ ) {
		dest[i] = 0;
		for( j = 0; j < cols; j++ ) {
			dest[i] += b[i][j] * a[j];
		}
	}
}


/**
 * Inverse matrix.
 * Works only for the cases 2x2 and 3x3.
 * @param int dest[DIM][DIM]
 * @param const int src[DIM][DIM]
 */
void matrix_inv( int dest[DIM][DIM], const int src[DIM][DIM] ) {
	switch( DIM ) {
		case 2:
			matrix_modinv_2x2( (int (*)[2]) dest, (const int (*)[2]) src, MODN );
			break;
		case 3:
			matrix_modinv_3x3( (int (*)[3]) dest, (const int (*)[3]) src, MODN );
			break;
		default:
			printf( "ERROR: Matrix in form %dx%d not supported.\n", DIM, DIM );
			exit( EXIT_FAILURE );
	}
}


/**
 *
 * @param int (*dest)[DIM]
 * @param char *src
 * @param int blocks
 */
void msg_to_blocks( int (*dest)[DIM], char *src, int blocks ) {
	int i, j, k = 0;

	for( i = 0; i < blocks; i++ ) {
		for( j = 0; j < DIM; j++ ) {
			dest[i][j] = ( k < strlen( src ) ) ? (int) src[k++] - 'A' : 0;
		}
	}
}


/**
 *
 * @param char *src
 * @return int
 */
int count_blocks_needed( char *src ) {
	int blocks = strlen( src ) / DIM;

	return ( strlen( src ) % DIM != 0 ) ? blocks + 1 : blocks;
}


/**
 * Encrypt hill cipher.
 * @param char *plain Plaintext to encrypt.
 * @param int *key Key to use.
 */
void encrypt( char *enc, char *plain, const int key[DIM][DIM] ) {
	int i, j, k = 0;
	int m_enc[strlen( plain )];
	int b = count_blocks_needed( plain );
	int msg_blocks[b][DIM];

	msg_to_blocks( msg_blocks, plain, b );

	for( j = 0; j < b; j++ ) {
		matrix_mult( m_enc, msg_blocks[j], key, DIM, DIM );

		for( i = 0; i < DIM; i++ ) {
			m_enc[i] = modulo( m_enc[i], MODN );
			enc[k] = m_enc[i] + 'A';
			k++;
		}
	}
	enc[k] = '\0';
}


/**
 * Decrypt hill cipher.
 * @param char *decr
 * @param char *encrypted
 * @param const int key[DIM][DIM]
 */
void decrypt( char *decr, char *encrypted, const int key[DIM][DIM] ) {
	int i, j, k = 0;
	int inv_key[DIM][DIM];
	int m_decr[strlen( encrypted )];
	int b = count_blocks_needed( encrypted );
	int msg_blocks[b][DIM];

	msg_to_blocks( msg_blocks, encrypted, b );
	matrix_inv( inv_key, key );

	for( i = 0; i < DIM; i++ ) {
		for( j = 0; j < DIM; j++ ) {
			inv_key[i][j] = modulo( inv_key[i][j], MODN );
		}
	}

	for( j = 0; j < b; j++ ) {
		matrix_mult( m_decr, msg_blocks[j], (const int (*)[DIM]) inv_key, DIM, DIM );

		for( i = 0; i < DIM; i++ ) {
			m_decr[i] = m_decr[i] % MODN;
			decr[k++] = m_decr[i] + 'A';
		}
	}
	decr[k] = '\0';
}


int main( void ) {
	char plaintext[] = "CLOUDSDALE";
	char encrypted[strlen( plaintext )];
	char decrypted[strlen( plaintext )];
	char decrypted_2[strlen( SECRET_TEXT )];

	// Encrypt
	encrypt( encrypted, plaintext, KEY );
	printf( "Plaintext: %s\n", plaintext );
	printf( "Encrypted: %s\n", encrypted );

	// Decrypt my encryption
	decrypt( decrypted, encrypted, KEY );
	printf( "Decrypted: %s\n", decrypted );
	printf( " ---------- \n" );

	// Decrypt given encryption
	decrypt( decrypted_2, (char*) SECRET_TEXT, KEY );
	printf( "Encrypted: %s\n", SECRET_TEXT );
	printf( "Decrypted: %s\n", decrypted_2 );

	return EXIT_SUCCESS;
}
