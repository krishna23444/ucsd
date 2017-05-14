/*
 * pa3.h
 */

#ifndef PA3_H
#define PA3_H

#define E_FLAG 0x01
#define I_FLAG 0x02
#define O_FLAG 0x04
#define B_FLAG 0x08
#define H_FLAG 0x10
#define F_FLAG 0x20

#define MIN_BASE 2
#define MAX_BASE 36

void printEnglish( long num );
void printIntBinary( long num );
void printFPBinary( float num );
void printBase( long num, int base );
int checkRange( long value, long minRange, long maxRange );
void processIntToken( char *token, int ibase, int obase, unsigned int mode );
void processFPToken( char *token, unsigned int mode );

#endif /* PA3_H */
