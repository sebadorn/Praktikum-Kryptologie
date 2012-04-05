#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define BUFSIZE 80
#define KEYMAXLENGTH 20

const char *ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


/**
 * This must look like a C code abomination from the depth
 * of hell for everyone with a little more knowledge than I have.
 *
 * Seriously, there has to be an easier way for basically everything
 * I did here. After writing 5 for-loops or so, which more less do the
 * same thing, I'm kind of sure about that.
 */


/**
 * Reads the content of a text file.
 * @param char* msg_before Destination of the text file content.
 * @param char* filename Name of the text file to read.
 */
void read_msg_file( char* msg_before, char* filename ) {
	FILE *msg_file;
	char buffer[BUFSIZE];

	// Open file
	msg_file = fopen( filename, "r" );
	if( msg_file == NULL ) {
		printf( "Could not open file %s.\n", filename );
		exit( EXIT_FAILURE );
	}

	// Read content of file
	while( fgets( buffer, BUFSIZE, msg_file ) != NULL ) {
		strcat( msg_before, buffer );
	}
	fclose( msg_file );
}


/**
 * Uses "msg-useable-b.txt" as input.
 */
int main( int argc, char* argv[] ) {
	char *msg_before;
	char *filename;
	// Keyphrase as given by user.
	char *keyphrase;
	// Keyphrase without whitespace and no doubled characters.
	char key_plain[KEYMAXLENGTH + 1];
	// Alphabet shifted and with included "plain" keyphrase.
	char code_abc[strlen( ABC ) + 1];
	char code_abc_shifted[strlen( ABC ) + 1];
	char decrypt[strlen( ABC ) + 1];
	// How much to shift the alphabet.
	int shift;
	int i, j, k, skip, code_letter;
	struct stat st;

	// Read parameters
	if( argc < 4 ) {
		printf( "ERROR: Missing parameters.\n" );
		printf( "    filename \t- Name of TXT file with the message.\n" );
		printf( "    keyphrase\t- Key phrase that has been used.\n" );
		printf( "    shift    \t- Number of positions the alphabet has been shifted.\n" );
		return EXIT_FAILURE;
	}

	filename = argv[1];
	keyphrase = argv[2];
	shift = 26 - atoi( argv[3] );

	if( strlen( keyphrase ) > KEYMAXLENGTH ) {
		printf( "ERROR: Keyphrase too long. Limit it at %d characters.", KEYMAXLENGTH );
		return EXIT_FAILURE;
	}


	// Remove whitespace from keyphrase
	k = 0;
	for( i = 0; i < strlen( keyphrase); i++ ) {
		// ASCII 32 == whitespace
		if( (int) keyphrase[i] == 32 ) {
			continue;
		}

		// No dublicate characters
		skip = 0;
		for( j = 0; j < k; j++ ) { // Char already in key_plain?
			if( key_plain[j] == keyphrase[i] ) {
				skip = 1;
				break;
			}
		}

		if( skip == 0 ) {
			key_plain[k++] = keyphrase[i];
		}
	}
	key_plain[k] = '\0';
	printf( "  Keyphrase: %s\n", key_plain );


	// Add keyphrase to begin of alphabet
	// Once again: No character shall be doubled.
	strcpy( code_abc, key_plain );
	k = strlen( code_abc );
	for( i = 0; i < strlen( ABC ); i++ ) {
		skip = 0;
		for( j = 0; j < k; j++ ) {
			if( code_abc[j] == ABC[i] ) {
				skip = 1;
				break;
			}
		}

		if( skip == 0 ) {
			code_abc[k++] = ABC[i];
		}
	}
	code_abc[26] = '\0';
	printf( "  Code alphabet: %s\n", code_abc );


	// Shift new alphabet
	k = shift;
	for( i = 0; i < strlen( code_abc ); i++ ) {
		code_abc_shifted[k] = code_abc[i];
		k = ( k + 1 ) % 26;
	}
	code_abc_shifted[26] = '\0';
	printf( "  Code alphabet shifted: %s\n", code_abc_shifted );


	// Allocate memory for the message
	if( stat( filename, &st ) == -1 ) {
		printf( "ERROR: stat() returning -1.\n" );
		return EXIT_FAILURE;
	}
	msg_before = malloc( st.st_size );

	// Read the message
	read_msg_file( msg_before, filename );


	// Find decryption alphabet
	k = 0;
	for( i = 0; i < strlen( ABC ); i++ ) {
		for( j = 0; j < strlen( code_abc_shifted ); j++ ) {
			if( code_abc_shifted[j] == ABC[i] ) {
				decrypt[k++] = ABC[j];
			}
		}
	}
	decrypt[k] = '\0';
	printf( "  Decryption alphabet: %s\n", decrypt );


	// Decrypt
	printf( "  Message:\n\n" );
	for( i = 0; i < strlen( msg_before ); i++ ) {
		if( msg_before[i] == '\n' ) {
			continue;
		}
		code_letter = decrypt[msg_before[i] - 65];
		code_letter += 32; // to lowercase

		printf( "%c", code_letter );
	}
	printf( "\n\n" );


	free( msg_before );
	return EXIT_SUCCESS;
}