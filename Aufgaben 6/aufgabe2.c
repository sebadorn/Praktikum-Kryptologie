#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../lib/crypto.h"

#define TRUE 1
#define FALSE 0

// Our alphabet: a-z,|,!,:,-,? (0-30)

long long KEY[3][3]; // "HILLKEY!"
int MODN = 0; // 31


/**
 *
 * @param long long dest[3][3]
 * @param char *src
 * @param int blocks
 * @param int flag_decipher TRUE or FALSE.
 */
void msg_to_blocks( long long dest[3][3], char *src, int flag_decipher ) {
	int j, k, m, val;
	long long xor = 0;

	m = 0;
	for( j = 0; j < 3; j++ ) {
		for( k = 0; k < 3; k++ ) {
			// Parity later
			if( flag_decipher == FALSE && j == 1 && k == 1 ) {
				continue;
			}

			switch( src[m] ) {
				case '|': val = 26; break;
				case '!': val = 27; break;
				case ':': val = 28; break;
				case '-': val = 29; break;
				case '?': val = 30; break;
				default:  val = src[m] - 'a';
			}

			m++;
			dest[j][k] = val;
			//sum += dest[j][k];
			xor ^= dest[j][k];
		}
	}

	if( flag_decipher == FALSE ) {
		dest[1][1] = ModS12( xor, MODN );
	}
}


/**
 * Encrypt a hill cipher.
 * @param char *enc Result: the cipher.
 * @param char *plain Message to encrypt.
 */
void hillverH33( char *enc, char *plain ) {
	int j, k = 0, t;
	long long m_enc[3][3];
	long long msg_blocks[3][3];
	long long val;

	msg_to_blocks( msg_blocks, plain, FALSE );
	matrix_mult( m_enc, KEY, msg_blocks, 3 );

	t = 0;
	for( j = 0; j < 3; j++ ) {
		for( k = 0; k < 3; k++ ) {
			m_enc[j][k] = ModS12( m_enc[j][k], MODN );

			switch( m_enc[j][k] ) {
				case 26: val = '|' - 'a'; break;
				case 27: val = '!' - 'a'; break;
				case 28: val = ':' - 'a'; break;
				case 29: val = '-' - 'a'; break;
				case 30: val = '?' - 'a'; break;
				default: val = m_enc[j][k];
			}
			enc[t++] = val + 'a';
		}
	}
	enc[t] = '\0';
}


/**
 * Decrypt the hill cipher again.
 * @param char *decr Result: the message.
 * @param char *encrypted Cipher to decrypt.
 */
void hillentH33( char *decr, char *encrypted ) {
	int i, j, k, t;
	double inv_key[3][3];
	long long inv_key_ll[3][3];
	long long m_decr[3][3];
	long long msg_blocks[3][3];

	msg_to_blocks( msg_blocks, encrypted, TRUE );
	matrix_inv( inv_key, KEY, 3, MODN );

	for( i = 0; i < 9; i++ ) {
		(*inv_key_ll)[i] = (long long) (*inv_key)[i];
	}

	matrix_mult( m_decr, inv_key_ll, msg_blocks, 3 );

	t = 0; k = 0;
	for( j = 0; j < 3; j++ ) {
		for( k = 0; k < 3; k++ ) {
			// Ignore parity
			if( j == 1 && k == 1 ) {
				continue;
			}
			m_decr[j][k] = ModS12( m_decr[j][k], MODN );
			decr[t++] = m_decr[j][k] + 'a';
		}
	}
	decr[t] = '\0';
}


int main( int argc, char *argv[] ) {
	char *msg; // "lestersh"
	char *ciph;
	int i, j, k;
	long long val, key_det;
	//long long sum = 0;
	int xor = 0;

	if( argc < 4 ) {
		printf( "  ERROR: Not enough parameters.\n" );
		return EXIT_FAILURE;
	}

	msg = argv[2];
	ciph = malloc( strlen( msg ) + 2 );
	MODN = atoi( argv[3] );

	// Convert key from string to matrix.
	j = 0; k = 0;
	for( i = 0; i < strlen( argv[1] ); i++ ) {
		// Skip parity
		if( j == 1 && k == 1 ) {
			k++;
		}

		switch( argv[1][i] ) {
			case '|': val = 26; break;
			case '!': val = 27; break;
			case ':': val = 28; break;
			case '-': val = 29; break;
			case '?': val = 30; break;
			default:  val = argv[1][i] - 'A';
		}
		KEY[j][k] = val;
		//sum += KEY[j][k];
		xor ^= KEY[j][k];
		k++;

		if( k >= 3 ) {
			k = 0;
			j++;
		}
	}
	// Parity
	//KEY[1][1] = ModS12( sum, MODN );
	KEY[1][1] = ModS12( xor, MODN );

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