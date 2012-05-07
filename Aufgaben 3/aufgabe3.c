#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/crypto.h"


int main( void ) {
	unsigned long long
		// Part a)
		x = 7323,
		y = 2469,
		a = 97,
		b = 1589316386,
		z, eulphi, r,
		s1, s2,
		// Part b)
		n = 13439,
		p, q, m;
	long long phi_n, h, rt;

	// Part a)
	printf( "Part a)\n ----- ----- \n" );

	// Step 1: z = x * z
	z = x * y;
	printf( "Step 1: z = %llu\n", z );

	// Step 2: EulerPhi(z)
	eulphi = EulPhiS12( z );
	printf( "Step 2: phi(z) = %llu\n", eulphi );

	// Step 3: r = b mod EulerPhi(z)
	r = ModS12( b, eulphi );
	printf( "Step 3: r = %llu\n", r );

	// Step 4: S1 = a^r mod z
	s1 = ModExpS12( a, r, z );
	printf( "Step 4: s1 = %llu\n", s1 );

	// Step 5: S2 = a^b mod z
	s2 = ModExpS12( a, b, z );
	printf( "Step 5: s2 = %llu\n", s2 );


	// Part b)
	printf( "\nPart b)\n ----- ----- \n" );

	// Step 1: n = p * q
	phi_n = EulPhiS12( n );
	h = phi_n - n - 1;
	rt = sqrt( pow( h / 2, 2 ) - n );
	p = - h / 2 + rt;
	q = - h / 2 - rt;
	/* sub-optimal:
	for( i = 2; i < n; i++ ) {
		if( n % i == 0 && IsPrimeS12( i ) ) {
			q = i;
			p = n / q;
			break;
		}
	}
	*/
	printf( "Step 1: p = %llu  q = %llu\n", p, q );

	// Step 2: m = p^2
	m = p * p;
	printf( "Step 2: m = %llu\n", m );

	// Step 3: EulerPhi(m)
	eulphi = EulPhiS12( m );
	printf( "Step 3: phi(m) = %llu\n", eulphi );

	// Step 4: a = p^2 * (1 - 1 / p)
	a = p * ( p - 1 );
	printf( "Step 4: a = %llu\n", a );


	return EXIT_SUCCESS;
}
