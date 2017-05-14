/* This is the complete MAC program for doing: encryption and CRC. 

   The steps are:
   1. The CRC of the frame is calculated. 
   2. The frame is encrypted.
   3. The CRC from Step1. is appended to the encrypted frame from Step2.
   4. The CRC of this whole payload is calculated and appended to the frame.
*/

#include "crc_table.h"

// Function declarations

void wep_encrypt(int plaintext_len, unsigned char plaintext[],
                 unsigned char ciphertext[], unsigned char seed[]);
unsigned long get_crc(int no_of_bytes, unsigned char data[]);
extern void WriteDebuggerWindow(unsigned long*, unsigned int);


void wep_encrypt(int plaintext_len, unsigned char plaintext[],
                 unsigned char ciphertext[], unsigned char seed[]) {
  int i; 

  unsigned int *start_encrypt_register  = (unsigned int *)  0x01000004;
  unsigned int *plain_address_register  = (unsigned int *)  0x01000008;
  unsigned int *frame_size_register     = (unsigned int *)  0x0100000C;
  unsigned int *cipher_address_register = (unsigned int *)  0x01000010;
  unsigned char *seed_lsw_register      = (unsigned char *) 0x01000014;
  unsigned char *seed_msw_register      = (unsigned char *) 0x01000018;
  unsigned int *done_register           = (unsigned int *)  0x0100001C;
  unsigned char *plaintext_address      = (unsigned char *) 0x01000100;
  unsigned char *cipher_address         = (unsigned char *) 0x01000200;

  for (i = 0; i < plaintext_len; ++i)
    plaintext_address[i] = plaintext[i];

  *plain_address_register = (unsigned int)plaintext_address;

  *frame_size_register = plaintext_len;

  *cipher_address_register = (unsigned int)cipher_address;

  seed_lsw_register[0] = seed[0];
  seed_lsw_register[1] = seed[1];
  seed_lsw_register[2] = seed[2];
  seed_lsw_register[3] = seed[3];
  seed_msw_register[0] = seed[4];
  seed_msw_register[1] = seed[5];
  seed_msw_register[2] = seed[6];
  seed_msw_register[3] = seed[7];

  *start_encrypt_register = 1;
  *start_encrypt_register = 0;

  while (*done_register != 1)
    ;

  for (i = 0; i < plaintext_len; ++i)
    ciphertext[i] = cipher_address[i];
}


unsigned long get_crc(int no_of_bytes, unsigned char data[]) {
  unsigned long crc_temp = 0xFFFFFFFF;
  int i, index;

  index = ((int)(crc_temp >> 24)) & 0xFF;
  crc_temp = (crc_temp << 8) ^ crc_table[index];

  for (i = 0; i < no_of_bytes; i++) {
    index = ((int)(crc_temp >> 24) ^ data[i]) & 0xFF;       
    crc_temp = (crc_temp << 8) ^ crc_table[index];
  }
  crc_temp = crc_temp ^ 0xFFFFFFFF;

  return crc_temp; 
}


int TestMain() {
  // ensure that no functions using semihosting SWIs are linked in from the
  // C library
#pragma import(__use_no_semihosting_swi)

  // Initialize the plaintext data
  unsigned char plaintext[48] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
    0x00, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x00, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
    0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89,
    0x28, 0x47, 0x37, 0x86, 0x31, 0x7F, 0x4D, 0x86 };

  // Declare the ciphertext array. It has 8 more bytes for 4+4 bytes of CRC data
  unsigned char ciphertext[48+8]; 
  unsigned long crc; 
  int no_of_bytes = 48; 
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
  for (i = 0; i< 1; i++) {
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
    WriteDebuggerWindow((unsigned long*) ciphertext, no_of_bytes+8);
  }
  return 1;
}
