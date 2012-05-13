#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../lib/crypto.h"

#define START_KEY 170

const int BUF = 256;
unsigned char key_state = START_KEY;


/**
 * Very simple shift register without XOR.
 * @return unsigned char
 */
unsigned char shift_register() {
	unsigned char key = 0, bit;
	int i;

	for( i = 0; i < 8; i++ ) {
		bit = key_state & 1;
		key |= ( bit << i ) & (char) pow( 2, i );
		key_state = ( key_state >> 1 ) | ( bit << 7 );
	}

	return key;
}


int main( int argc, char *argv[] ) {
	FILE *infile, *outfile;
	char *infname, *outfname, msg[BUF], *c = malloc( sizeof( char ) );
	int i;

	// Read parameters
	if( argc < 3 ) {
		printf( "ERROR: Not enough parameters provided.\n" );
		return EXIT_FAILURE;
	}
	infname = argv[1];
	outfname = argv[2];

	// Read file
	infile = fopen( infname, "rb" );
	if( infile == NULL ) {
		printf( "ERROR: Could not open file to read: %s\n", infname );
		return EXIT_FAILURE;
	}
	fgets( msg, BUF, infile );
	fclose( infile );

	// Encrypt and write to file
	outfile = fopen( outfname, "wb" );
	if( outfile == NULL ) {
		printf( "ERROR: Could not open or create file to write: %s\n", outfname );
		return EXIT_FAILURE;
	}

	key_state = START_KEY; // Reset initial key
	for( i = 0; i < strlen( msg ); i++ ) {
		*c = msg[i] ^ shift_register();
		fwrite( c, 1, 1, outfile );
	}
	fclose( outfile );

	free( c );
	return EXIT_SUCCESS;
}