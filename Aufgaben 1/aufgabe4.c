#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/**
 * @param char* text
 * @param char* geheimnis
 */
void verschluesseler( char* text, char* geheimnis ) {
	char c;
	char k;
	int i;
	int j = 0;
	int g_length = strlen( geheimnis );

	for( i = 0; i < strlen( text ); i++ ) {
		c = text[i];
		k = geheimnis[j];

		text[i] = ( ( c - 32 ) ^ k ) + 32;

		j = ( j + 1 ) % g_length;
	}
}


int main( void ) {
	char text[] = "Dies ist der gegebene Klartext";
	char geheimnis[] = "GEHEIM";

	printf( "Vor der Verschlüsselung:  %s\n", text );
	verschluesseler( text, geheimnis );
	printf( "Nach der Verschlüsselung: %s\n", text );
	verschluesseler( text, geheimnis );
	printf( "Nach der Entschlüsselung: %s\n", text );

	return EXIT_SUCCESS;
}