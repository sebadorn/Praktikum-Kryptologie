#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../lib/crypto.h"


/**
 *
 * @param unsigend char ... a lot
 */
void blockcipher( unsigned char *c1, unsigned char *c2, unsigned char *c3, unsigned char *c4,
		unsigned char m1, unsigned char m2, unsigned char m3, unsigned char m4,
		unsigned char k1, unsigned char k2, unsigned char k3 ) {
	unsigned char
		a, b, c, m,
		a1 = 0, a2 = 0, a3 = 0, a4 = 0,
		b1 = 0, b2 = 0, b3 = 0, b4 = 0;

	// 240 = 11110000
	// 15  = 00001111

	m = ( m1 << 4 ) | m2; a = k1 ^ m;
	a1 = a & 240; a2 = a & 15;
	//printf( "a1||a2 = %d   ", a );

	m = ( m3 << 4 ) | m4; a = k1 ^ m;
	a3 = a & 240; a4 = a & 15;
	//printf( "a3||a4 = %d\n", a );


	a = a1 | ( a3 >> 4 ); b = k2 ^ a;
	b1 = b & 240; b2 = b & 15;
	//printf( "b1||b2 = %d  ", b );

	a = ( a2 << 4 ) | a4; b = k2 ^ a;
	b3 = b & 240; b4 = b & 15;
	//printf( "b3||b4 = %d\n", b );


	b = b1 | ( b3 >> 4 ); c = k3 ^ b;
	*c1 = c & 240; *c2 = c & 15;
	//printf( "c1||c2 = %d  ", c );

	b = ( b2 << 4 ) | b4; c = k3 ^ b;
	*c3 = c & 240; *c4 = c & 15;
	//printf( "c3||c4 = %d\n", c );

	*c1 = *c1 >> 4; *c3 = *c3 >> 4;
}


/**
 *
 * @param unsigned char *cpart
 * @param FILE *outfile
 */
void write_cipherpart( unsigned char *cpart, FILE *outfile ) {
	int i, j;
	char *t = malloc( sizeof( char ) );

	for( i = 3; i >= 0; i-- ) {
		j = *cpart & (int) pow( 2, i );
		j /= ( j == 0 ) ? 1 : j;
		*t = (char) ( j + '0' );
		fwrite( t, 1, 1, outfile );
	}
	*t = ' ';
	fwrite( t, 1, 1, outfile );

	free( t );
}


int main( int argc, char *argv[] ) {
	FILE *infile, *outfile;
	char *infilename, *outfilename, buffer[9];
	unsigned char
		*c1 = malloc( sizeof( char ) ), *c2 = malloc( sizeof( char ) ),
		*c3 = malloc( sizeof( char ) ), *c4 = malloc( sizeof( char ) ),
		k1 = 0, k2 = 0, k3 = 0,
		m1 = 0, m2 = 0, m3 = 0, m4 = 0;
	int i, j;


	// Get file names
	if( argc >= 3 ) {
		infilename = argv[1];
		outfilename = argv[2];
	}
	else {
		printf( "ERROR: Not enough parameters provided.\n" );
		return EXIT_FAILURE;
	}


	// File to read from
	infile = fopen( infilename, "r" );
	if( infile == NULL ) {
		printf( "ERROR: Could not open file to read: %s\n", infilename );
		return EXIT_FAILURE;
	}

	// Key 1
	fgets( buffer, 9, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		k1 |= j << ( 7 - i );
	}

	// Key 2
	fseek( infile, 9, SEEK_SET );
	fgets( buffer, 9, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		k2 |= j << ( 7 - i );
	}

	// Key 3
	fseek( infile, 18, SEEK_SET );
	fgets( buffer, 9, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		k3 |= j << ( 7 - i );
	}

	// Message part 1
	fseek( infile, 27, SEEK_SET );
	fgets( buffer, 5, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		m1 |= j << ( 3 - i );
	}

	// Message part 2
	fseek( infile, 32, SEEK_SET );
	fgets( buffer, 5, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		m2 |= j << ( 3 - i );
	}

	// Message part 3
	fseek( infile, 37, SEEK_SET );
	fgets( buffer, 5, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		m3 |= j << ( 3 - i );
	}

	// Message part 4
	fseek( infile, 42, SEEK_SET );
	fgets( buffer, 5, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		m4 |= j << ( 3 - i );
	}

	fclose( infile );


	// Execute the block cipher
	blockcipher(
		c1, c2, c3, c4,
		m1, m2, m3, m4,
		k1, k2, k3
	);


	// File to write to
	outfile = fopen( outfilename, "w" );
	if( outfile == NULL ) {
		printf( "ERROR: Could not open or create file to write: %s\n", outfilename );
		return EXIT_FAILURE;
	}

	// Write cipher parts
	write_cipherpart( c1, outfile );
	write_cipherpart( c2, outfile );
	write_cipherpart( c3, outfile );
	write_cipherpart( c4, outfile );

	fclose( outfile );

	free( c1 ); free( c2 );
	free( c3 ); free( c4 );
	return EXIT_SUCCESS;
}