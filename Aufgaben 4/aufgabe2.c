#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../lib/crypto.h"


const char REVERSE[] = "cipher-to-plain2.txt";


/**
 *
 * @param unsigned char k Value to shift.
 * @return unsigned char Shifted value.
 */
unsigned char shift_register( unsigned char k ) {
	unsigned char first5, last3, bit;

	first5 = k & 248; // 248 = 11111000
	last3 = k & 7;    // 7   = 00000111

	bit = first5 & 8; // 8   = 00001000
	first5 = ( ( first5 >> 1 ) & 248 ) | ( bit << 4 );

	bit = last3 & 1;  // 1   = 00000001
	last3 = ( last3 >> 1 ) | ( bit << 2 );

	return first5 ^ last3;
}


/**
 *
 * @param unsigned char mi Message to encrypt.
 * @param unsigned char ki Key to use for encryption.
 * @return unsigned char Cipher.
 */
unsigned char encrypt( unsigned char mi, unsigned char ki ) {
	return mi ^ ki;
}


/**
 *
 * @param unsigned char *cpart
 * @param FILE *outfile
 */
void write_cipherpart( unsigned char cpart, FILE *outfile ) {
	int i, j;
	unsigned char *t = malloc( sizeof( char ) );

	for( i = 7; i >= 0; i-- ) {
		j = cpart & (int) pow( 2, i );
		j /= ( j == 0 ) ? 1 : j;
		*t = (unsigned char) ( j + '0' );
		fwrite( t, 1, 1, outfile );
	}
	*t = ' ';
	fwrite( t, 1, 1, outfile );

	free( t );
}


int main( int argc, char *argv[] ) {
	FILE *infile, *outfile;
	char *infilename, *outfilename, buffer[9];
	unsigned char key_orig = 0, key, m[8], c[8];
	int i, j, l;

	// Read parameters
	if( argc < 3 ) {
		printf( "ERROR: Not enough parameters provided.\n" );
		return EXIT_FAILURE;
	}
	infilename = argv[1];
	outfilename = argv[2];


	// Read file
	infile = fopen( infilename, "rb" );
	if( infile == NULL ) {
		printf( "ERROR: Could not open file for reading: %s\n", infilename );
		return EXIT_FAILURE;
	}

	// Get key
	fgets( buffer, 9, infile );
	for( i = 0; i < strlen( buffer ); i++ ) {
		j = buffer[i] - '0';
		key_orig |= j << ( 7 - i );
	}
	printf( "  Key: %s\n", buffer );

	// Get message parts
	printf( "  m: " );
	for( i = 0; i < 8; i++ ) {
		fseek( infile, 9 * ( i + 1 ), SEEK_SET );
		fgets( buffer, 9, infile );
		m[i] = 0;
		for( l = 0; l < 8; l++ ) {
			j = buffer[l] - '0';
			m[i] |= j << ( 7 - l );
		}
		printf( "%d ", m[i] );
	}
	printf( "\n" );

	fclose( infile );


	// When does the key start repeating?
	i = 0;
	key = key_orig;
	while( 1 ) {
		key = shift_register( key );
		i++;
		if( key == key_orig ) {
			printf( "  Key repeats after %d shifts.\n", i );
			break;
		}
	}


	// Write cipher to file
	outfile = fopen( outfilename, "wb" );
	if( outfile == NULL ) {
		printf( "ERROR: Could not open or create file for writing: %s\n", outfilename );
		return EXIT_FAILURE;
	}

	printf( "  m->c: " );
	key = key_orig;
	for( i = 0; i < 8; i++ ) {
		c[i] = encrypt( m[i], key );
		key = shift_register( key );
		write_cipherpart( c[i], outfile );
		printf( "%d ", c[i] );
	}
	printf( "\n" );

	fclose( outfile );


	// Reverse (decrypt)
	outfile = fopen( REVERSE, "wb" );
	if( outfile == NULL ) {
		printf( "ERROR: Could not open or create file for writing: %s\n", REVERSE );
		return EXIT_FAILURE;
	}

	printf( "  c->m: " );
	key = key_orig;
	for( i = 0; i < 8; i++ ) {
		m[i] = encrypt( c[i], key );
		key = shift_register( key );
		write_cipherpart( m[i], outfile );
		printf( "%d ", m[i] );
	}
	printf( "\n" );

	fclose( outfile );


	return EXIT_SUCCESS;
}