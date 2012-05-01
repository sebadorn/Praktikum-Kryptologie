extern int ModInv( int a, int n );

extern int matrix_det_2x2( const int a[2][2] );
extern int matrix_det_3x3( const int a[3][3] );
extern void matrix_inv_2x2( int dest[2][2], const int src[2][2], int modn );
extern void matrix_inv_3x3( int dest[3][3], const int src[3][3], int modn );

extern int modulo( int a, int b );

extern unsigned long long AnzPrime( unsigned long long a, unsigned long long b );

extern int lcg( int seed, int a, int b, int m );

extern unsigned long long eEA1S12( unsigned long long a,
		unsigned long long b, long long *x, long long *y );
extern unsigned long long eEAdS12( unsigned long long a, unsigned long long b,
		unsigned long long d, long long *x, long long *y );
extern unsigned long long prime1S12( unsigned long long n );
extern unsigned long long primeZS12( unsigned long long a, unsigned long long b );
extern unsigned long long primeNS12( unsigned long long **ptr, unsigned long long n );
extern unsigned int IsPrimeS12( unsigned long long n );
extern unsigned long long RandS12( unsigned long long seed, unsigned long long a,
		unsigned long long b, unsigned long long m );
extern unsigned long long ggTS12( unsigned long long a, unsigned long long b );
extern unsigned long long kgVS12( unsigned long long a, unsigned long long b );
extern unsigned long long EulPhiS12( unsigned long long n );