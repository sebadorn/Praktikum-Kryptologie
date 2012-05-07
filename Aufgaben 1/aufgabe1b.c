#include<stdio.h>
#include<stdlib.h>


int main( int argc, char *argv[] ) {
	char *inputfile;
	char *outputfile;
	FILE *infile;
	FILE *outfile;
	char *c = NULL;

	// Get file names
	if( argc >= 3 ) {
		inputfile = argv[1];
		outputfile = argv[2];
	}
	else {
		printf( "ERROR: Not enough parameters provided.\n" );
		return EXIT_FAILURE;
	}

	// File to read from
	infile = fopen( inputfile, "r" );
	if( infile == NULL ) {
		printf( "ERROR: Could not open file to read: %s\n", inputfile );
		return EXIT_FAILURE;
	}

	// File to write to
	outfile = fopen( outputfile, "w" );
	if( outfile == NULL ) {
		printf( "ERROR: Could not open or create file to write: %s\n", outputfile );
		return EXIT_FAILURE;
	}

	// Copy content
	while( fread( c, 1, 1, infile ) == 1 ) {
		fwrite( c, 1, 1, outfile );
	}

	// Close files
	fclose( infile );
	fclose( outfile );

	return EXIT_SUCCESS;
}