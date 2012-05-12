#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../lib/crypto.h"


const int BUF = 256;
const unsigned char START_KEY = 170; // 170 = 10101010


/**
 * Very simple shift register without XOR.
 * @param unsigned char k
 * @return unsigned char
 */
unsigned char shift_register( unsigned char k ) {
	return ( k >> 1 ) | ( k & 1 );
}


int main( int argc, char *argv[] ) {
	FILE *infile, *outfile;
	char *infname, *outfname, msg[BUF], *c = malloc( sizeof( char ) );
	unsigned char key;
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

	key = START_KEY;
	for( i = 0; i < strlen( msg ); i++ ) {
		*c = msg[i] ^ key;
		key = shift_register( key );
		fwrite( c, 1, 1, outfile );
	}
	fclose( outfile );

	free( c );
	return EXIT_SUCCESS;
}