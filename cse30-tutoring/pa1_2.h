#ifndef PA1_H
#define PA1_H

/* Local function prototypes for PA1 (written in Assembly or C) */

long strToLong( char* str, int base );
int checkRange( long value, long minRange, long maxRange );
void displayX( long size, long xChar, long fillerChar, long borderChar );
int isEven( long value );

#define BASE 10
#define X_SIZE_MIN 4
#define X_SIZE_MAX 5000

/*
 * See "man ascii" for ASCII char values
 */
#define ASCII_MIN 32				/* ' ' */
#define ASCII_MAX 126				/* '~' */

/*
 * void printChar( char ch );
 * int numOfDigits( long num );
 *
 * Only called from an Assembly routine. Not needed in any C routine.
 * Would get a lint message about function declared but not used.
 */

#endif /* PA1_H */
