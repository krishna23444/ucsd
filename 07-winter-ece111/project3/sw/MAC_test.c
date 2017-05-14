/* This is the complete MAC program for doing: encryption and CRC. 

   The steps are:
   1. The CRC of the frame is calculated. 
   2. The frame is encrypted.
   3. The CRC from Step1. is appended to the encrypted frame from Step2.
   4. The CRC of this whole payload is calculated and appended to the frame.
*/

#include "crc_table.h"

// Function declarations

void sbox_setup ( unsigned char seed[] );
unsigned char get_wep_byte ( void );
void wep_encrypt ( int plaintext_len, unsigned char plaintext[], unsigned char \
	ciphertext[], unsigned char seed[] );
unsigned long get_crc( int no_of_bytes, unsigned char data[] );
extern void WriteDebuggerWindow( unsigned long*, unsigned int);


// Global Variable declarations

static unsigned char sbox[256];
static unsigned char wep_index1;
static unsigned char wep_index2;


void sbox_setup(unsigned char seed[])
{
        unsigned char index, temp;
        short counter;

        // Initialize index variables
        index = 0;
        wep_index1 = 0;
        wep_index2 = 0;

        for (counter = 0; counter < 256; counter++)
                sbox[counter] = (unsigned char)counter;

        for (counter = 0; counter < 256; counter++)
        {
                // Calculate index
                index = (index + sbox[counter] + seed[counter % 8]) % 256;

                // Swap bytes
                temp = sbox[counter];
                sbox[counter] = sbox[index];
                sbox[index] = temp;
        }
}

unsigned char get_wep_byte(void)
{
        unsigned char index, temp;

        wep_index1 = (wep_index1 + 1) % 256;
        wep_index2 = (wep_index2 + sbox[wep_index1]) % 256;
        temp = sbox[wep_index1];
        sbox[wep_index1] = sbox[wep_index2];
        sbox[wep_index2] = temp;
        index = (sbox[wep_index1] + sbox[wep_index2]) % 256;

        return sbox[index];
}


void wep_encrypt(int plaintext_len, unsigned char plaintext[], unsigned char ciphertext[], unsigned char seed[])
{
        int i;

        sbox_setup(seed);

        for (i = 0; i < plaintext_len; i++)
        {
                ciphertext[i] = plaintext[i] ^ get_wep_byte();
        }
}


unsigned long get_crc(int no_of_bytes, unsigned char data[])
{
        unsigned long crc_temp = 0xFFFFFFFF;
        int i, index;

        index = ((int)(crc_temp >> 24)) & 0xFF;
        crc_temp = (crc_temp << 8) ^ crc_table[index];

        for (i = 0; i < no_of_bytes; i++)
        {
           index = ((int)(crc_temp >> 24) ^ data[i]) & 0xFF;       
           crc_temp = (crc_temp << 8) ^ crc_table[index];
        }
        crc_temp = crc_temp ^ 0xFFFFFFFF;

        return crc_temp; 
}
  

int TestMain()
{
// ensure that no functions using semihosting SWIs are linked in from the C library
#pragma import(__use_no_semihosting_swi)


	// Initialize the plaintext data
  /*
        unsigned char plaintext[48] =  
	       { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                 0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86 };
                 */
        unsigned char plaintext[256] =  
	       { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
           0x28, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x4D, 0x86 };

	// Declare the ciphertext array. It has 8 more bytes for 4+4 bytes of CRC data
//        unsigned char ciphertext[56]; 
        unsigned char ciphertext[256+8]; 
//        int no_of_bytes = 48; 
        int no_of_bytes = 256; 
        unsigned long crc; 
        int i; 
	unsigned char seed[8];

	// The 5 MSBs of the seed do not change with the frame
	// The 3 LSBs change with each new frame (part of specification)
	seed[3] = 0x67;
	seed[4] = 0x89;
	seed[5] = 0xAB;
	seed[6] = 0xCD;
	seed[7] = 0xEF;

	// For project-3 we're applying the MAC protocol to only one frame
	// to have manageable simulation time

        for (i = 0; i< 1; i++)
        {
	  seed[0] = 0x01;
	  seed[1] = 0x23;
	  seed[2] = 0x45;

	  // Compute the CRC of the plaintext
          crc = get_crc(no_of_bytes, plaintext); 

	  // Encrypt the plaintext
          wep_encrypt(no_of_bytes, plaintext, ciphertext, seed); 

	  // Append the CRC to the ciphertext
          ciphertext[no_of_bytes]   = (unsigned char)(crc >> 24);
          ciphertext[no_of_bytes+1] = (unsigned char)(crc >> 16);
          ciphertext[no_of_bytes+2] = (unsigned char)(crc >> 8);
          ciphertext[no_of_bytes+3] = (unsigned char)(crc);

	  // Compute the CRC of the whole payload
          crc = get_crc(no_of_bytes+4, ciphertext);         

	  // Append the CRC to the payload
          ciphertext[no_of_bytes+4]   = (unsigned char)(crc >> 24);
          ciphertext[no_of_bytes+4+1] = (unsigned char)(crc >> 16);
          ciphertext[no_of_bytes+4+2] = (unsigned char)(crc >> 8);
          ciphertext[no_of_bytes+4+3] = (unsigned char)(crc);

	  // Display the final frame which would be transmitted
	  // Cannot have printf statements in code
	  //fprintf(stdout, "%x\n", ciphertext[j]);

	  // Display is handled by XRAY debugger
	  // You'll have to single step through the code for this function
	  // There is some bug in XRAY which causes it to crash if you
	  // just run through this.
	  // Set a breakpoint to just before this function and then single step
	  // through the code to display the final frame which would be transmitted 
	  // Alternately, you can also view the appropriate memory location
	  // of ciphertext to see the final data.

	  WriteDebuggerWindow( (unsigned long*) ciphertext, no_of_bytes+8 );

        }
return 1;
}

