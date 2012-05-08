extern int modulo( int a, int b );

extern unsigned long long AnzPrime( unsigned long long a, unsigned long long b );

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
extern unsigned long long DivS12( unsigned long long a, unsigned long long n );
extern unsigned long long ModS12( unsigned long long a, unsigned long long n );
extern unsigned long long ModInvS12( unsigned long long a, unsigned long long n );
extern unsigned long long ModExpS12( unsigned long long a, unsigned long long b,
		unsigned long long n );

extern long long matrix_det( long long **a, unsigned int n, unsigned long long mod );
extern void matrix_adj( double **dest, long long **a, unsigned int n, unsigned long long mod );
extern void matrix_inv( double **dest, long long **a, unsigned int n, unsigned long long mod );