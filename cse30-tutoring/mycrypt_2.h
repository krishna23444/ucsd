#ifndef MYCRYPT_H
#define MYCRYPT_H

#define PASS_PHRASE_SIZE 8
#define WHITE_SPACE_CHARS " \t\n"

#define MIN_ROTATE -63
#define MAX_ROTATE +63

extern FILE *devtty;

/* Local function prototypes for MYCRYPT (written in Assembly or C) */

void getPassPhrase( char passPhrase[] );
void getKeys( unsigned long keys[] );
int getRotateValue( void );
void maskPassPhrase( unsigned long keys[], char passPhrase[],
		     unsigned long mask[] );
void mycrypt( FILE *inFile, unsigned long mask[], int rotateValue );
unsigned long strToULong( char* str, int base );
int checkRange( long value, long minRange, long maxRange );

/*
 * void rotate( unsigned long mask[], int rotateCnt );
 *
 * Used in assembly routine mycrypt().
 * Commented out here to keep lint happy and as documentation.
 */

#endif /* MYCRYPT_H */
