#include <stdio.h>

int j; 
void sbox_setup(int seed_msw, int seed_lsw);
int get_wep_byte(void);

static unsigned char sbox[256];
static unsigned char wep_index1;
static unsigned char wep_index2;

int main()
{
	int seed_msw; 
        int seed_lsw; 

	unsigned char plaintext1[64] = 
	{	0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
                0x28, 0x00, 0x00, 0x00, 
		0x00, 0x7F, 0x4D, 0x86 };

	unsigned char plaintext2[64] = 
        {	0x00, 0x00, 0x00, 0x00,
        	0x78, 0x56, 0x34, 0x12,
        	0xF0, 0xDE, 0xBC, 0x9A,
        	0x00, 0x00, 0x00, 0x00,
        	0x00, 0x00, 0x00, 0x00,
        	0x30, 0x50, 0x00, 0x0F,
        	0x28, 0x00, 0x00, 0x00,
        	0x00, 0x7f, 0x4d, 0x86,
        	0x02, 0x70, 0x36, 0x82,
        	0x30, 0x45, 0x56, 0x04,
        	0x22, 0x57, 0x64, 0x45,
        	0x52, 0xC3, 0x56, 0xAB,
        	0x22, 0x53, 0x55 };

	unsigned char frame_size;
	unsigned char ciphertext1[64];
	unsigned char ciphertext2[64];

	int i;
	
// Encrypt Frame 1	

	seed_msw = 0xEFCDAB89;
	seed_lsw = 0x67452301;
	frame_size = 48;

	sbox_setup(seed_msw, seed_lsw);

	for (i = 0; i < frame_size; i++)
	{
		ciphertext1[i] = plaintext1[i] ^ (unsigned char)get_wep_byte();
		
	}

	printf("---------\n"); 
	printf("Plaintext1\n"); 
	printf("---------\n"); 
	for (i = 0; i < frame_size; i = i+4)
	{
	   printf("%x %x %x %x\n", plaintext1[i+3], plaintext1[i+2], plaintext1[i+1], plaintext1[i]); 
	}

	printf("----------\n"); 
	printf("Ciphertext1\n"); 
	printf("----------\n"); 
	for (i = 0; i < frame_size; i = i+4)
	{
	   printf("%x %x %x %x\n", ciphertext1[i+3], ciphertext1[i+2], ciphertext1[i+1], ciphertext1[i]); 
	}

// Encrypt Frame 2

        seed_msw = 0xEFCDAB89;
        seed_lsw = 0x673678AB; // note, last three bytes have changed
        frame_size = 51;

        sbox_setup(seed_msw, seed_lsw);

        for (i = 0; i < frame_size; i++)
        {
                ciphertext2[i] = plaintext2[i] ^ (unsigned char)get_wep_byte();

        }

        printf("---------\n");
        printf("Plaintext2\n");
        printf("---------\n");
        for (i = 0; i < frame_size; i = i+4)
        {
           printf("%x %x %x %x\n", plaintext2[i+3], plaintext2[i+2], plaintext2[i+1], plaintext2[i]);
        }

        printf("----------\n");
        printf("Ciphertext2\n");
        printf("----------\n");
        for (i = 0; i < frame_size; i = i+4)
        {
           printf("%x %x %x %x\n", ciphertext2[i+3], ciphertext2[i+2], ciphertext2[i+1], ciphertext2[i]);
        }

	return 0;
}

void sbox_setup(int seed_msw, int seed_lsw)
{
        unsigned char index, temp;
        unsigned char seed[8];
        short counter;

        // Initialize index variables
        index = 0;
        wep_index1 = 0;
        wep_index2 = 0;

        // Convert seed to byte array
        seed[7] = (unsigned char)(seed_msw >> 24);
        seed[6] = (unsigned char)(seed_msw >> 16);
        seed[5] = (unsigned char)(seed_msw >> 8);
        seed[4] = (unsigned char)(seed_msw);
        seed[3] = (unsigned char)(seed_lsw >> 24);
        seed[2] = (unsigned char)(seed_lsw >> 16);
        seed[1] = (unsigned char)(seed_lsw >> 8);
        seed[0] = (unsigned char)(seed_lsw);

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

int get_wep_byte(void)
{
        unsigned char index, temp;

        wep_index1 = (wep_index1 + 1) % 256;
        wep_index2 = (wep_index2 + sbox[wep_index1]) % 256;
        temp = sbox[wep_index1];
        sbox[wep_index1] = sbox[wep_index2];
        sbox[wep_index2] = temp;
        index = (sbox[wep_index1] + sbox[wep_index2]) % 256;

        return (int)sbox[index];
}

