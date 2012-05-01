extern int ggT( int a, int b );
extern int kgV( int a, int b );
extern int ModInv( int a, int n );

extern int matrix_det_2x2( const int a[2][2] );
extern int matrix_det_3x3( const int a[3][3] );
extern void matrix_inv_2x2( int dest[2][2], const int src[2][2], int modn );
extern void matrix_inv_3x3( int dest[3][3], const int src[3][3], int modn );

extern int modulo( int a, int b );

extern int EulerPhi( int n );
extern int lcg( int seed, int a, int b, int m );
extern unsigned long long eEA1S12( unsigned long long a,
		unsigned long long b, long long *x, long long *y );
extern unsigned long long eEAdS12( unsigned long long a, unsigned long long b,
		unsigned long long d, long long *x, long long *y );
extern unsigned long long prime1S12( unsigned long long n );