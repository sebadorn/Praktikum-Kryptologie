#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

#include "../lib/crypto.h"


const char chiffre_file[] = "cipher.txt";
const char plain[] = "TheQuickBrownFoxJumpsOverTheLazyDog.";
int xstart[8] = { 1, 0, 1, 0, 1, 0, 1, 0 };


/**
 *
 * @param int dest[8]
 * @param int x[8]
 */
void shift_register_8( unsigned int dest[8], unsigned int x[8] ) {
	int T[8][8] = {
		{ 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 1, 0, 0, 0, 0 }
	};
	int i, j, idx;

	for( i = 0; i < 8; i++ ) {
		idx = 7 - i;
		dest[idx] = 0;
		for( j = 0; j < 8; j++ ) {
			dest[idx] += x[7 - j] * T[i][j];
		}
		dest[idx] = dest[idx] % 2;
	}
}


/**
 *
 * @param int a[8]
 * @return unsigned char
 */
unsigned char bits_to_char( unsigned int a[8] ) {
	int i, c = 0;

	for( i = 0; i < 8; i++ ) {
		//c += a[i] * pow( 2.0, 7 - i );
		c += a[i] << ( 7 - i );
	}

	return (unsigned char) c;
}


/**
 *
 * @param unsigned char *dest
 * @param unsigned char *filename
 */
void read_chiffre( unsigned char *dest, const char *filename ) {
	FILE *src = fopen( filename, "rb" );
	long pos;

	if( src == NULL ) {
		printf( "Couldn't open file.\n" );
		exit( EXIT_FAILURE );
	}

	fseek( src, 0, SEEK_END );
	pos = ftell( src );
	fseek( src, 0, SEEK_SET );

	fread( dest, pos, 1, src );
	fclose( src );
}


/**
 *
 * @param unsigned char *dest
 * @param unsigned char *plain
 */
void stream_cipher( unsigned char *dest, unsigned char *plain ) {
	unsigned int x[8], key[8];
	//unsigned int bit[8], c;
	unsigned int i, j;
	unsigned char key_char;

	for( i = 0; i < 8; i++ ) {
		key[i] = xstart[i];
	}


	for( i = 0; i < strlen( (const char *)plain ); i++ ) {
		/*
		// New key from the shift register
		shift_register_8( key, x );

		// Get bits of char
		c = (unsigned int) plain[i];
		bit[0] = c / 128; c -= ( bit[0] * 128 );
		bit[1] = c / 64;  c -= ( bit[1] * 64 );
		bit[2] = c / 32;  c -= ( bit[2] * 32 );
		bit[3] = c / 16;  c -= ( bit[3] * 16 );
		bit[4] = c / 8;   c -= ( bit[4] * 8 );
		bit[5] = c / 4;   c -= ( bit[5] * 4 );
		bit[6] = c / 2;   c -= ( bit[6] * 2 );
		bit[7] = c;

		// XOR char bits and key
		c = 0;
		for( j = 0; j < 8; j++ ) {
			//printf( "bit[%d] = ( %d + %d ) %% 2 = ", j, bit[j], key[j] );
			bit[j] = ( bit[j] + key[j] ) % 2;
			//printf( "%d\n", bit[j] );
			c += bit[j] * pow( 2.0, 7 - j );
		}

		dest[i] = (unsigned char) c;
		//printf( "%d -> %d\n", plain[i], dest[i] );

		// Key will be seed for the next key
		for( j = 0; j < 8; j++ ) {
			x[j] = key[j];
		}
		*/

		key_char = bits_to_char( key );

		dest[i] = plain[i] ^ key_char;

		for( j = 0; j < 8; j++ ) {
			x[j] = key[j];
		}
		shift_register_8( key, x );
	}
}


int main( int argc, char *argv[] ) {
	struct stat st;
	unsigned char *chiffre;
	unsigned char *chiffre_decrypted;
	unsigned char encrypted[strlen( plain ) + 1];
	unsigned char decrypted[strlen( plain ) + 1];


	// Test with encryption and decryption
	stream_cipher( encrypted, (unsigned char *)plain );
	printf( "----------\n" );
	printf( "%s\n-> %s\n", plain, encrypted );
	printf( "----------\n" );
	stream_cipher( decrypted, encrypted );
	printf( "%s\n-> %s\n", encrypted, decrypted );
	printf( "----------\n" );

	// Use it on given message

	if( stat( chiffre_file, &st ) == -1 ) {
		printf( "ERROR: stat() returning -1.\n" );
		return EXIT_FAILURE;
	}
	chiffre = malloc( st.st_size );
	chiffre_decrypted = malloc( st.st_size );

	// Read message
	read_chiffre( chiffre, chiffre_file );

	// Decrypt it
	stream_cipher( chiffre_decrypted, chiffre );
	printf( "%s\n----------\n", chiffre_decrypted );


	free( chiffre_decrypted );
	free( chiffre );
	return EXIT_SUCCESS;
}