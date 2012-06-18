#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

#include "../lib/crypto.h"

#define BUFSIZE 2

const double N = 65521.0;
const double K = 17.0;
//const char MSG_FILE[] = "text_Q11_linux.txt";
const char MSG_FILE[] = "min_test.txt";


/**
 *
 * @param unsigned char *dest
 * @param const char *filename
 */
void read_msg( unsigned char *dest, const char *filename ) {
	char buffer[BUFSIZE];
	FILE *src;

	// Get contents
	src = fopen( filename, "r" );
	if( src == NULL ) {
		printf( "Could not open file %s.\n", filename );
		exit( EXIT_FAILURE );
	}

	while( fgets( buffer, BUFSIZE, src ) != NULL ) {
		if( buffer[0] == '\r' || buffer[0] == '\n' ) {
			continue;
		}
		strcat( (char *) dest, buffer );
	}

	fclose( src );
}


/**
 *
 * @param unsigned char *msg
 * @return unsigned short Hash value.
 */
unsigned short hash( unsigned char *msg ) {
	unsigned short len, blocks, i;
	long double m = 0, c;
	unsigned long long tmp;

	len = strlen( (char *)msg );
	blocks = len / 2 + len % 2;
	printf( "  Msg length: %d\n", len );
	printf( "  Blocks: %d\n", blocks );

	m = ( msg[0] << 8 ) | msg[1];
	c = pow( m, K );
	c = fmod( c, N );
	i = 2;
	while( i < len ) {
		tmp = ( msg[i] << 8 ) | msg[i + 1];
		tmp ^= (unsigned long long) c;
		m = (long double) tmp;
		c = pow( m, K );
		c = fmod( c, N );
		i += 2;
	}

	return c;
}


int main( void ) {
	struct stat st;
	unsigned char *msg;
	unsigned short h;

	// Get space for the message.
	if( stat( MSG_FILE, &st ) == -1 ) {
		printf( "ERROR: stat() returning -1.\n" );
		return EXIT_FAILURE;
	}
	msg = malloc( st.st_size );

	read_msg( msg, MSG_FILE );
	h = hash( msg );

	printf( "  ----- -----\n  Hash: %04X\n", h );

	// Clean up
	free( msg );

	return EXIT_SUCCESS;
}