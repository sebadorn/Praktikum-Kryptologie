#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/crypto.h"

int t = 0; // t = 5  // t = 17
int k = 0; // k = 7  // k = 4
int n = 0; // n = 26 // n = 256
int min_char = 256;


/**
 *
 * @param int t Value to find multiplicative inverse of.
 * @param int n Modulo.
 * @return int A multiplicative inverse of t.
 */
int MultInv( int t, int n ) {
	int i;

	for( i = 1;; i++ ) {
		if( ModS12( n * i, t ) == t - 1 ) {
			return ( n * i + 1 ) / t;
		}
	}
}


/**
 *
 * @param unsigned char *dest
 * @param unsigend char *msg
 */
void Affine( unsigned char *dest, unsigned char *msg ) {
	int i, z;

	for( i = 0; i < strlen( (const char *)msg ); i++ ) {
		z = msg[i] - min_char;
		dest[i] = ModS12( z * t + k, n );
	}
}


/**
 *
 * @param unsigned char *dest
 * @param unsigned char *cipher
 */
void AffineInv( unsigned char *dest, unsigned char *cipher ) {
	int i;
	int b = MultInv( t, n );
	int l = ModS12( b * ( n - k ), n );

	printf( "  - Multiplicative inverse of %d mod %d = %d\n", t, n, b );
	printf( "  - l = %d\n", l );

	for( i = 0; i < strlen( (const char *)cipher ); i++ ) {
		dest[i] = (int) ModS12( b * cipher[i] + l, n );
		dest[i] += min_char;
	}
}


int main( int argc, char *argv[] ) {
	int i;
	unsigned char *msg;
	unsigned char *ciph;

	if( argc < 5 ) {
		printf( "  ERROR: Not enough parameters provided.\n" );
		printf( "  Parameters: t, k, n, msg\n" );
		return EXIT_FAILURE;
	}

	t = atoi( argv[1] );
	k = atoi( argv[2] );
	n = atoi( argv[3] );

	if( ggTS12( t, n ) != 1 ) {
		printf( "  ERROR: t and n have to be coprime.\n" );
		return EXIT_FAILURE;
	}

	msg = (unsigned char *)argv[4];
	ciph = malloc( sizeof( char ) * strlen( (const char *)msg ) );

	for( i = 0; i < strlen( (const char *)msg ); i++ ) {
		min_char = ( msg[i] < min_char ) ? msg[i] : min_char;
	}
	printf( "  - Min char to shift by: %d\n", min_char );

	printf( "  t = %d, k = %d, n = %d\n", t, k, n );
	printf( "  Message: %s\n", msg );
	Affine( ciph, msg );
	printf( "  Cipher: %s\n", ciph );
	AffineInv( msg, ciph );
	printf( "  Back to message: %s\n", msg );

	free( ciph );
	return EXIT_SUCCESS;
}