/*
 * This is a way to print each byte we encrypted as 2 hex characters
 * if we wanted to check the encrypted output as ASCII chars.
 */

#ifdef NOTDEF
      (void) fprintf( stderr, "\n" );
      for ( i = 0; (i + 4) < length; i += 4 )
         (void) fprintf( stderr, "%08X", *(unsigned long *)(buf + i) );

      for ( j = 0; j < (length - i); ++j )
         (void) fprintf( stderr, "%02X", (num = *(buf + i + j)) );
      (void) fprintf( stderr, "\n" );
#endif /* NOTDEF */
  
