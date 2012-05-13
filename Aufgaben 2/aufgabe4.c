#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

#include "../lib/crypto.h"

#define KEY_START 170

const char chiffre_file[] = "cipher_D8.txt";
const char plain[] = "TheQuickBrownFoxJumpsOverTheLazyDog.";
unsigned char key = KEY_START; // 170 = 10101010



/**
 * Shift register.
 * @return unsigned char Shifted value.
 */
unsigned char shift_register_8() {
	unsigned char bit1, bit4, bit8_new, k = 0;
	int i;

	for( i = 0; i < 8; i++ ) {
		k |= ( key << i ) & (char) pow( 2, i );

		bit1 = key & 1; // 1 = 00000001
		bit4 = ( key >> 3 ) & 1;
		bit8_new = ( bit1 ^ bit4 ) << 7;
		key = ( key >> 1 ) | bit8_new;
	}

	return k;
}


/**
 *
 * @param char *dest
 * @param const char *filename
 */
void read_chiffre( char *dest, const char *filename ) {
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
 * @param char *dest
 * @param char *plain
 */
void stream_cipher( char *dest, char *plain ) {
	int i, limit = strlen( (const char *)plain );

	for( i = 0; i < limit; i++ ) {
		dest[i] = ( plain[i] ^ shift_register_8() );
	}
	dest[i] = '\0';
}


int main( void ) {
	struct stat st;
	char
		*chiffre,
		*chiffre_decrypted,
		encrypted[strlen( plain ) + 1],
		decrypted[strlen( plain ) + 1];

	// Test with encryption and decryption
	key = KEY_START;
	stream_cipher( encrypted, (char *)plain );
	printf( "----------\n" );
	printf( "%s\n%s\n", plain, encrypted );
	printf( "----------\n" );

	key = KEY_START;
	stream_cipher( decrypted, encrypted );
	printf( "%s\n%s\n", encrypted, decrypted );
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
	key = KEY_START;
	stream_cipher( chiffre_decrypted, chiffre );
	printf( "%s\n----------\n", chiffre_decrypted );


	free( chiffre_decrypted );
	free( chiffre );
	return EXIT_SUCCESS;
}