#include <stdio.h>
#include <stdlib.h>

#include "../lib/crypto.h"


int main( void ) {
	int f;
	f = ModInv( 4, 17 );
	printf( "ModInv( 4, 17 ) == %d\n", f );

	return EXIT_SUCCESS;
}