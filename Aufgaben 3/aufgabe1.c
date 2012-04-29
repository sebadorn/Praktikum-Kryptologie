#include <stdio.h>
#include <stdlib.h>


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
 * Count number of primes in an interval.
 * @param int a Interval start.
 * @param int b Interval end.
 * @return int Number of primes in the given interval.
 */
int AnzPrime( int a, int b ) {
	int i, j, count = 0;
	int sieve[b + 1];

	for( i = 2; i <= b; i++ ) {
		sieve[i] = 1;
	}

	for( i = 2; i <= b; i++ ) {
		if( sieve[i] == 1 ) {
			if( i >= a ) {
				count++;
			}
			for( j = i * i; j <= b; j += i ) {
				sieve[j] = 0;
			}
		}
	}

	return count;
}


/**
 *
 * @param int z
 * @return int
 */
int IsPrime( int z ) {
	int i, j, is_prime = -1;
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
	int a = 10;
	int b = 40;
	int prime = 367;
	int not_prime = 244;

	Prime( a, b );
	printf( "Primes in [%d, %d]: %d\n", a, b, AnzPrime( a, b ) );
	printf( "Is %d a prime? -> %d\n", prime, IsPrime( prime ) );
	printf( "Is %d a prime? -> %d\n", not_prime, IsPrime( not_prime ) );

	return EXIT_SUCCESS;
}