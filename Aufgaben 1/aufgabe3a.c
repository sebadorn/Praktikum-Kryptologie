#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define BUFSIZE 80


/**
 * Reads the content of a text file.
 * @param char* msg_before Destination of the text file content.
 * @param char* filename Name of the text file to read.
 */
void read_msg_file( char *msg_before, char *filename ) {
	FILE *msg_file;
	char buffer[BUFSIZE];

	// Open file
	msg_file = fopen( filename, "r" );
	if( msg_file == NULL ) {
		printf( "Could not open file %s.\n", filename );
		exit( EXIT_FAILURE );
	}

	// Read content of file
	while( fgets( buffer, BUFSIZE, msg_file ) != NULL ) {
		strcat( msg_before, buffer );
	}
	fclose( msg_file );
}


/**
 * Uses "msg-useable-a.txt" as input.
 * Solution through trying 1-25 as shift value results in 23.
 */
int main( int argc, char *argv[] ) {
	char *msg_before;
	char *filename;
	char c;
	struct stat st;
	int shift;
	int i;
	int ascii;
	size_t msg_length;

	// Read parameters
	if( argc < 3 ) {
		printf( "ERROR: Missing parameters.\n" );
		return EXIT_FAILURE;
	}

	filename = argv[1];
	shift = atoi( argv[2] );

	if( shift < 1 || shift > 25 ) {
		printf( "ERROR: Value for shift should be between 1 and 25. Input was %d.\n", shift );
		return EXIT_FAILURE;
	}


	// Allocate memory for the message
	if( stat( filename, &st ) == -1 ) {
		printf( "ERROR: stat() returning -1.\n" );
		return EXIT_FAILURE;
	}
	msg_before = malloc( st.st_size );


	// Read the message
	read_msg_file( msg_before, filename );


	// Shift message
	msg_length = strlen( msg_before );

	for( i = 0; i < msg_length; i++ ) {
		if( msg_before[i] == '\n' ) {
			continue;
		}

		// Shift character
		ascii = (int) msg_before[i] + shift;

		// Wrap alphabetical area
		if( ascii > 90 ) { // Z == 90, A == 65
			ascii = 64 + ( ascii % 90 );
		}

		// To lowercase
		ascii += 32;

		printf( "%c", (char) ascii );
	}
	printf( "\n" );


	free( msg_before );
	return EXIT_SUCCESS;
}