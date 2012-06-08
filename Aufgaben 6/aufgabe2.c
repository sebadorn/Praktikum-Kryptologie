#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../lib/crypto.h"

long long KEY[3][3]; // "HILLKEY!"
int MODN = 0; // 31


/**
 * Multiply two matrices (or a matrix and a vector).
 * @param long long dest_mat[3][3] Matrix with the result.
 * @param long long a_mat[3][3] Matrix a.
 * @param double b_mat[3][3] Matrix b.
 */
void matrix_mult( long long dest_mat[3][3], long long a_mat[3][3], long long b_mat[3][3] ) {
	int i, j, k = 0;

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			dest_mat[i][j] = 0;
			for( k = 0; k < 3; k++ ) {
				dest_mat[i][j] += a_mat[i][k] * b_mat[k][j];
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
			if( i == 1 && j == 1 ) {
				continue;
			}

			dest[i][j] = ( k < strlen( src ) ) ? (int) src[k++] - 'A' : 0;
			sum += dest[i][j];
		}
		dest[1][1] = ModS12( sum, MODN );
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

	return ModS12( strlen( src ), 8 ) != 0 ? blocks + 1 : blocks;
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
				m_enc[j][k] = ModS12( m_enc[j][k], MODN );
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
	matrix_inv( (double (*)[3]) inv_key, KEY, 3, MODN );

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			inv_key[i][j] = ModS12( inv_key[i][j], MODN );
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
				m_decr[j][k] = ModS12( m_decr[j][k], MODN );
				decr[t++] = m_decr[j][k] + 'A';
			}
		}
	}
	decr[t] = '\0';
}


int main( int argc, char *argv[] ) {
	char *msg; // "lestersh"
	char *ciph;
	int i, j, k;
	long long sum, key_det;

	if( argc < 4 ) {
		printf( "  ERROR: Not enough parameters.\n" );
		return EXIT_FAILURE;
	}

	msg = argv[2];
	ciph = malloc( sizeof( char ) * strlen( msg ) );

	MODN = atoi( argv[3] );

	// Convert key from string to matrix.
	j = 0; k = 0;
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
	KEY[1][1] = ModS12( sum, MODN );

	// For decryption, gcd( det KEY, MODN ) has to equal 1.
	key_det = matrix_det( KEY, 3, MODN );
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