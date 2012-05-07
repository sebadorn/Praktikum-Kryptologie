#include <stdio.h>
#include <stdlib.h>

#include "../lib/crypto.h"


/**
 *
 * @param int a
 * @param int b
 */
void Prime( int a, int b ) {
	int i, j;
	int sieve[b + 1];

	for( i = 2; i <= b; i++ ) {
		sieve[i] = 1;
	}

	for( i = 2; i <= b; i++ ) {
		if( sieve[i] == 1 ) {
			if( i >= a ) {
				printf( "Prime: %d\n", i );
			}
			for( j = i * i; j <= b; j += i ) {
				sieve[j] = 0;
			}
		}
	}
}


/**
 *
 * @param int z
 * @return int
 */
int IsPrime( int z ) {
	int i, j;
	int sieve[z + 1];

	for( i = 2; i <= z; i++ ) {
		sieve[i] = 1;
	}

	for( i = 2; i <= z; i++ ) {
		if( sieve[i] == 1 ) {
			if( i == z ) {
				return 1;
			}
			for( j = i * i; j <= z; j += i ) {
				if( j == z ) {
					return -1;
				}
				sieve[j] = 0;
			}
		}
	}

	return -1;
}


int main( void ) {
	unsigned long long a = 10, b = 40;
	int prime = 367;
	int not_prime = 244;

	Prime( a, b );
	printf( "Primes in [%llu, %llu]: %llu\n", a, b, AnzPrime( a, b ) );
	printf( "Is %d a prime? -> %d\n", prime, IsPrime( prime ) );
	printf( "Is %d a prime? -> %d\n", not_prime, IsPrime( not_prime ) );

	return EXIT_SUCCESS;
}