#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/crypto.h"

const char SECRET[] = "UBIXGTENERHLNHAHRM";
const int KEY[] = { 0, 1, 7, 15 };


/**
 *
 * @param const char* secret_in
 * @param int* block_secret
 */
void secret_to_blocks( const char* secret_in, int* block_secret ) {
	int i;

	for( i = 0; i < strlen( secret_in ); i++ ) {
		block_secret[i] = (int) secret_in[i] - 'A';
		//printf( "%d, ", block_secret[i] );
	}
}


/**
 *
 * @param int* block_secret
 */
void decrypt( int* block_secret ) {
	// C = P * ( K % n ); // encrypt
	// P = C / ( K % n ); // ?
}


int main( void ) {
	int n = 27;
	int dim = 2;
	int blocks;
	int *block_secret = (int*) malloc( strlen( SECRET ) * sizeof( int ) );

	blocks = strlen( SECRET ) / 4;
	if( strlen( SECRET ) % 4 > 0 ) {
		blocks++;
	}
	printf( "Blocks needed: %d\n", blocks );

	secret_to_blocks( SECRET, block_secret );
	decrypt( block_secret );

	free( block_secret );
	return EXIT_SUCCESS;
}