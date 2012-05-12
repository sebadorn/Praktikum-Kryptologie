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
const unsigned char KEY_START = 170; // 170 = 10101010



/**
 * Shift register.
 * @param unsigned char k Value to shift.
 * @return unsigned char Shifted value.
 */
unsigned char shift_register_8( unsigned char k ) {
	unsigned char bit1, bit4, bit8_new;

	bit1 = k & 1; // 1 = 00000001
	bit4 = ( k & 8 ) >> 3; // 8 = 00001000
	bit8_new = ( bit1 ^ bit4 ) << 7;

	return ( k >> 1 ) | bit8_new;
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
	unsigned char key = KEY_START;
	int i;

	for( i = 0; i < strlen( (const char *)plain ); i++ ) {
		dest[i] = ( plain[i] ^ key );
		key = shift_register_8( key );
	}
	dest[i] = '\0';
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