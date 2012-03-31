#include<stdio.h>
#include<stdlib.h>


int ggT( int a, int b ) {
	if( b == 0 ) {
		return a;
	}
	return ggT( b, a % b );
}


int kgV( int a, int b ) {
	return a * b / ggT( a, b );
}


int main( int argc, char *argv[] ) {
	int a;
	int b;

	if( argc >= 3 ) {
		// get integer
		a = atoi( argv[1] );
		b = atoi( argv[2] );

		// only positive values
		a = ( a < 0 ) ? a * (-1) : a;
		b = ( b < 0 ) ? b * (-1) : b;

		printf( "a = %d\nb = %d\n", a, b );

		if( a == 0 && b == 0 ) {
			printf( "ERROR: Not both values can be 0.\n" );
			return EXIT_FAILURE;
		}
	}
	else {
		printf( "ERROR: Not enough parameters. Expected two integers.\n" );
		return EXIT_FAILURE;
	}

	printf( "ggT(a,b): %d\n", ggT( a, b ) );
	printf( "kgV(a,b): %d\n", kgV( a, b ) );

	return EXIT_SUCCESS;
}