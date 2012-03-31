#include<stdio.h>
#include<stdlib.h>


int main( int argc, char *argv[] ) {
	int i;

	printf( "Parameter: " );
	for( i = 0; i < argc; i++ ) {
		printf( "%s ", argv[i] );
	}
	printf( "\n" );

	return EXIT_SUCCESS;
}