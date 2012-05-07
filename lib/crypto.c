#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "crypto.h"



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


/**
 * Calculate determinant of a n x n matrix.
 * @param long long **a Matrix.
 * @param unsigned int n Length of matrix.
 * @return long long Determinant of matrix.
 */
long long matrix_det( long long **a, unsigned int n ) {
	long long det, i, j, k, l;
	long long **m = NULL;

	// Not possible
	if( n < 1 ) {
		printf( "ERROR: Matrix cannot be smaller than n = 1!" );
		exit( EXIT_FAILURE );
	}

	// 1 x 1
	if( n == 1 ) {
		det = a[0][0];
	}
	// 2 x 2
	else if( n == 2 ) {
		det = a[0][0] * a[1][1] - a[1][0] * a[0][1];
	}
	// n x n
	else {
		det = 0;
		for( i = 0; i < n; i++ ) {
			// Get memory for temporary matrix
			m = malloc( ( n - 1 ) * sizeof( long long *) );
			for( j = 0; j < n; j++ ) {
				m[j] = malloc( ( n - 1 ) * sizeof( long long ) );
			}

			for( j = 1; j < n - 1; j++ ) {
				k = 0;
				for( l = 0; l < n; l++ ) {
					if( l == i ) {
						continue;
					}
					m[j - 1][k] = a[j][l];
					k++;
				}
			}

			det += pow( -1.0, 2.0 + i ) * a[0][i] * matrix_det( m, n - 1 );

			for( j = 0; j < n - 1; j++ ) {
				free( m[j] );
			}
			free( m );
		}
	}

	return det;
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
 * Linear Congruential Generator.
 * @param unsigned long long seed Start value x0.
 * @param unsigned long long a Multiplier.
 * @param unsigned long long b Increment.
 * @param unsigned long long m Modulus.
 * @return unsigned long long Pseudo-random value.
 */
unsigned long long RandS12( unsigned long long seed, unsigned long long a,
		unsigned long long b, unsigned long long m ) {
	return modulo( ( a * seed + b ), m );
}


/**
 * Find the ggT using the euclidean algorithm.
 * @param unsigned long long a
 * @param unsigned long long b
 * @return unsigned long long
 */
unsigned long long ggTS12( unsigned long long a, unsigned long long b ) {
	if( b == 0 ) {
		return a;
	}
	return ggTS12( b, modulo( a, b ) );
}


/**
 * Finds the kgV using the ggT (using the euclidean algorithm).
 * @param unsigned long long a
 * @param unsigned long long b
 * @return unsigned long long
 */
unsigned long long kgVS12( unsigned long long a, unsigned long long b ) {
	return a * b / ggTS12( a, b );
}


/**
 * Euler's phi function.
 * @param unsigned long long n
 * @return unsigned long long Number of natural numbers smaller than n and coprime to n.
 */
unsigned long long EulPhiS12( unsigned long long n ) {
	unsigned long long k, ep = 0;

	if( n < 2 ) {
		return 0;
	}

	if( IsPrimeS12( n ) ) {
		return n - 1;
	}

	for( k = 0; k < n; k++ ) {
		if( ggTS12( k, n ) == 1 ) {
			ep++;
		}
	}

	return ep;
}


/**
 * DivS12. Seems a little pointless?
 * @param unsigned long long a
 * @param unsigned long long n
 * @return unsigned long long
 */
unsigned long long DivS12( unsigned long long a, unsigned long long n ) {
	return a / n;
}


/**
 * ModS12.
 * @param unsigned long long a
 * @param unsigned long long n
 * @return unsigned long long
 */
unsigned long long ModS12( unsigned long long a, unsigned long long n ) {
	//return ( a % n + n ) % n; // If negative were possible.
	return a % n;
}


/**
 * Modular inverse using the extended euclidean algorithm.
 * @param unsigned long long a
 * @param unsigned long long n
 * @return unsigned long long
 */
unsigned long long ModInvS12( unsigned long long a, unsigned long long n ) {
	unsigned long long n_orig = n, r;
	long long
		x = 0, y = 1,
		u = 1, v = 0,
		b, m;
	double q;

	while( a != 0 ) {
		q = n / a;
		r = ModS12( n, a );
		m = x - u * q;
		b = y - v * q;
		n = a;
		a = r;
		x = u;
		y = v;
		u = m;
		v = b;
	}

	if( n != 1 ) {
		return 0;
	}
	return ModS12( x, n_orig );
}


/**
 * Modular exponent.
 * @param unsigned long long a
 * @param unsigned long long b
 * @param unsigned long long n
 * @return unsigned long long
 */
unsigned long long ModExpS12( unsigned long long a, unsigned long long b,
		unsigned long long n ) {
	unsigned long long d = 1;

	while( b > 0 ) {
		if( ( b & 1 ) == 1 ) {
			d = ( d * a ) % n;
		}
		b = b >> 1;
		a = ( a * a ) % n;
	}

	return d;
}