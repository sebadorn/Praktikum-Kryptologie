#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include "../lib/crypto.h"


const char chiffre_file[] = "chiffre-4.txt";
const unsigned char plain[] = "CLOUDSDALE";
int xstart[8] = { 1, 0, 1, 0, 1, 0, 1, 0 };


/**
 *
 * @param int dest[8]
 * @param int x[8]
 */
void shift_register_8( int dest[8], int x[8] ) {
	int T[8][8] = {
		0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0
	};
	int i, j;

	for( i = 0; i < 8; i++ ) {
		dest[i] = 0;
		for( j = 0; j < 8; j++ ) {
			dest[i] += x[j] * T[i][j];
		}
		dest[i] = dest[i] % 2;
	}
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
	int i, j, b, bit, k;
	int x[8], key[8];

	for( i = 0; i < 8; i++ ) {
		x[i] = xstart[i];
	}

	// TODO: This won't work. At least not the way it's supposed to.
	for( i = 0; i < strlen( plain ); i++ ) {
		k = 0;
		shift_register_8( key, x );

		for( bit = 1 << ( 8 - 1 ); bit; bit >>= 1 ) { // 8 -> CHAR_BIT
			b = ( plain[i] & bit ) ? 1 : 0;
			b = ( b + key[k++] ) % 2;
			dest[i] |= b << k; // Set bit k to b.
		}

		for( j = 0; j < 8; j++ ) {
			x[j] = key[j];
		}
	}
}


int main( void ) {
	struct stat st;
	unsigned char *chiffre;
	unsigned char encrypted[strlen( plain )];
	unsigned char decrypted[strlen( plain )];
	int xnew[8];
	int i;

	shift_register_8( xnew, xstart );

	for( i = 0; i < 8; i++ ) {
		printf( "%d ", xnew[i] );
	}
	printf( "\n" );


	// Allocate memory for the message
	if( stat( chiffre_file, &st ) == -1 ) {
		printf( "ERROR: stat() returning -1.\n" );
		return EXIT_FAILURE;
	}
	chiffre = malloc( st.st_size );
	//read_chiffre( chiffre, chiffre_file );

	// Guess what? NOT WORKING! D:
	stream_cipher( encrypted, (unsigned char*) plain );
	printf( "----------\n" );
	printf( "%s\n", encrypted );
	printf( "----------\n" );
	stream_cipher( decrypted, encrypted );
	printf( "%s\n", decrypted );
	printf( "----------\n" );

	free( chiffre );
	return EXIT_SUCCESS;
}