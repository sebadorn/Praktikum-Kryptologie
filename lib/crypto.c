#include <stdlib.h>
#include <stdio.h>

#include "crypto.h"


/**
 * ggT
 * @param int a
 * @param int b
 * @return int
 */
int ggT( int a, int b ) {
	if( b == 0 ) {
		return a;
	}
	return ggT( b, modulo( a, b ) );
}


/**
 * kgV
 * @param int a
 * @param int b
 * @return int
 */
int kgV( int a, int b ) {
	return a * b / ggT( a, b );
}


/**
 * Modular inverse.
 * @param int a
 * @param int n
 * @return int
 */
int ModInv( int a, int n ) {
	int i;

	for( i = 1; i < n - 1; i++ ) {
		if( modulo( i * a, n ) == 1 ) {
			return i;
		}
	}
	return 0;
}


/**
 * Calculate determinant of a 2x2 matrix.
 * @param const int a[2][2]
 * @return int Determinant of matrix.
 */
int matrix_det_2x2( const int a[2][2] ) {
	return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}


/**
 * Calculate determinant of a 3x3 matrix.
 * @param const int a[3][3]
 * @return int Determinant of matrix.
 */
int matrix_det_3x3( const int a[3][3] ) {
	int plus = a[0][0] * a[1][1] * a[2][2]
			 + a[0][1] * a[1][2] * a[2][0]
			 + a[0][2] * a[1][0] * a[2][1];
	int minus = a[0][2] * a[1][1] * a[2][0]
			 + a[1][2] * a[2][1] * a[0][0]
			 + a[2][2] * a[0][1] * a[1][0];
	return plus - minus;
}


/**
 * Invert a 2x2 matrix.
 * @param int dest[2][2]
 * @param int src[2][2]
 * @param int modn
 */
void matrix_inv_2x2( int dest[2][2], const int src[2][2], int modn ) {
	int i, j, f;

	f = ModInv( matrix_det_2x2( src ), modn );

	dest[0][0] = f * src[1][1];
	dest[0][1] = -f * src[0][1];
	dest[1][0] = -f * src[1][0];
	dest[1][1] = f * src[0][0];
}


/**
 * Invert a 3x3 matrix.
 * @param int dest[3][3]
 * @param int src[3][3]
 * @param int modn
 */
void matrix_inv_3x3( int dest[3][3], const int src[3][3], int modn ) {
	int i, j, f;

	f = ModInv( matrix_det_3x3( src ), modn );

	dest[0][0] = f * ( src[1][1] * src[2][2] - src[1][2] * src[2][1] );
	dest[0][1] = f * ( src[0][2] * src[2][1] - src[0][1] * src[2][2] );
	dest[0][2] = f * ( src[0][1] * src[1][2] - src[0][2] * src[1][1] );

	dest[1][0] = f * ( src[1][2] * src[2][0] - src[1][0] * src[2][2] );
	dest[1][1] = f * ( src[0][0] * src[2][2] - src[0][2] * src[2][0] );
	dest[1][2] = f * ( src[0][2] * src[1][0] - src[0][0] * src[1][2] );

	dest[2][0] = f * ( src[1][0] * src[2][1] - src[1][1] * src[2][0] );
	dest[2][1] = f * ( src[2][0] * src[0][1] - src[0][0] * src[2][1] );
	dest[2][2] = f * ( src[0][0] * src[1][1] - src[0][1] * src[1][0] );
}


/**
 * Modulo.
 * Spot the difference.
 * Python:	-4 % 17 = 13 (good)
 * C:		-4 % 17 = -4 (bad)
 * @param int a
 * @param int b
 * @return int
 */
int modulo( int a, int b ) {
	while( a < 0 ) {
		a += b;
	}
	return a % b;
}


/**
 * Linear Congruential Generator.
 * @param int seed Start value x0.
 * @param int a Multiplier.
 * @param int b Increment.
 * @param int m Modulus.
 * @return int Pseudo-random value.
 */
int lcg( int seed, int a, int b, int m ) {
	return modulo( ( a * seed + b ), m );
}


/**
 * Count number of primes in an interval.
 * @param unsigned long long a Interval start.
 * @param unsigned long long b Interval end.
 * @return unsigned long long Number of primes in the given interval.
 */
unsigned long long AnzPrime( unsigned long long a, unsigned long long b ) {
	unsigned long long i, j, count = 0;
	unsigned long long sieve[b + 1];

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



// Exercise 3


/**
 * Extended euclidean algorithm.
 * @param unsigned long long a
 * @param unsigned long long b
 * @param long long *x
 * @param long long *y
 * @return unsigned long long
 */
unsigned long long eEA1S12( unsigned long long a,
		unsigned long long b, long long *x, long long *y ) {
	long long ggT, q, r,
		u = 1, v = 0, s = 0, t = 1;

	while( b > 0 ) {
		q = a / b;
		r = a - q * b; a = b; b = r;
		r = u - q * s; u = s; s = r;
		r = v - q * t; v = t; t = r;
	}
	ggT = a;
	*x = u;
	*y = v;

	return ggT;
}


/**
 * eEAdS12
 * @param unsigned long long a
 * @param unsigned long long b
 * @param unsigned long long d
 * @param long long *x
 * @param long long *y
 * @return unsigned long long
 */
unsigned long long eEAdS12( unsigned long long a, unsigned long long b,
		unsigned long long d, long long *x, long long *y ) {
	long long ggT, q, r,
		u = 1, v = 0, s = 0, t = 1;

	while( b > 0 ) {
		q = a / b;
		r = a - q * b; a = b; b = r;
		r = u - q * s; u = s; s = r;
		r = v - q * t; v = t; t = r;
	}
	ggT = a;

	s = d / ggT;
	if( d == 0 ) {
		return 0;
	}
	*x = s * u;
	*y = s * v;

	return ggT;
}


/**
 * Returns the value of the n-th prime number.
 * @param unsigned long long n
 * @return unsigned long long
 */
unsigned long long prime1S12( unsigned long long n ) {
	int flag_is_prime;
	unsigned long long i, j, primes_found = 0, nth_prime = 0;

	i = 2;
	while( nth_prime == 0 ) {
		flag_is_prime = 1;
		for( j = 2; j < i; j++ ) {
			if( i % j == 0 ) {
				flag_is_prime = -1;
				break;
			}
		}

		if( flag_is_prime == 1 ) {
			primes_found++;
			if( primes_found == n ) {
				nth_prime = i;
			}
		}
		i += ( i == 2 ) ? 1 : 2;
	}

	return nth_prime;
}


/**
 * Return randomly chosen prime number from the given interval [a, b].
 * @param unsigned long long a Interval (inclusive) start.
 * @param unsigned long long b Interval (inclusive) end.
 * @return unsigned long long Randomly chosen prime number.
 */
unsigned long long primeZS12( unsigned long long a, unsigned long long b ) {
	unsigned long long
		i, j, count = 0, sieve[b + 1],
		rand_prime, take_nth_prime;

	srand( time( NULL ) );

	// Init sieve.
	for( i = 2; i <= b; i++ ) {
		sieve[i] = 1;
	}

	// Cross out not-primes
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

	// Take one of the found primes
	take_nth_prime = rand() % count;
	j = 0;
	for( i = 0; i <= b; i++ ) {
		if( sieve[i] == 1 ) {
			if( j == take_nth_prime ) {
				rand_prime = i;
				break;
			}
			j++;
		}
	}

	return rand_prime;
}


/**
 * Checks if n is a prime.
 * @param unsigned long long n
 * @return unsigned int 1 if n is a prime, 0 otherwise.
 */
unsigned int IsPrimeS12( unsigned long long n ) {
	unsigned long long i;

	for( i = 2; i < n; i++ ) {
		if( n % i == 0 ) {
			return 0;
		}
	}

	return 1;
}


/**
 * Calculates all primes p with 2 <= p <= n.
 * @param unsigned long long **ptr
 * @param unsigned long long n
 * @return unsigned long long Number of primes p <= n.
 */
unsigned long long primeNS12( unsigned long long **ptr, unsigned long long n ) {
	unsigned long long
		i, j, p = 0, sieve[n + 1],
		primes = AnzPrime( 2, n );

	*ptr = calloc( primes, sizeof( unsigned long long ) );

	if( *ptr == NULL ) {
		printf( "Memory allocation for ptr failed.\n" );
	}


	for( i = 2; i <= n; i++ ) {
		sieve[i] = 1;
	}

	for( i = 2; i <= n; i++ ) {
		if( sieve[i] == 1 ) {
			(*ptr)[p++] = i;
			for( j = i * i; j <= n; j += i ) {
				sieve[j] = 0;
			}
		}
	}

	return primes;
}


/**
 * Euler's phi function.
 * @param unsigned long long n
 * @return unsigned long long Number of natural numbers smaller than n and coprime to n.
 */
unsigned long long EulPhiS12( unsigned long long n ) {
	unsigned long long k, ep = 0;

	for( k = 0; k < n; k++ ) {
		if( ggT( k, n ) == 1 ) {
			ep++;
		}
	}
	return ep;
}