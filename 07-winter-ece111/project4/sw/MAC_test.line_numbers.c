0     /* This is the complete MAC program for doing: encryption and CRC. 
1     
2        The steps are:
3        1. The CRC of the frame is calculated. 
4        2. The frame is encrypted.
5        3. The CRC from Step1. is appended to the encrypted frame from Step2.
6        4. The CRC of this whole payload is calculated and appended to the frame.
7     */
8     
9     #include "crc_table.h"
10    
11    // Function declarations
12    
13    void wep_encrypt(int plaintext_len, unsigned char plaintext[],
14                     unsigned char ciphertext[], unsigned char seed[]);
15    unsigned long get_crc(int no_of_bytes, unsigned char data[]);
16    extern void WriteDebuggerWindow(unsigned long*, unsigned int);
17    
18    
19    void wep_encrypt(int plaintext_len, unsigned char plaintext[],
20                     unsigned char ciphertext[], unsigned char seed[]) {
21      int i; 
22    
23      unsigned int *start_encrypt_register  = (unsigned int *)  0x01000004;
24      unsigned int *plain_address_register  = (unsigned int *)  0x01000008;
25      unsigned int *frame_size_register     = (unsigned int *)  0x0100000C;
26      unsigned int *cipher_address_register = (unsigned int *)  0x01000010;
27      unsigned char *seed_lsw_register      = (unsigned char *) 0x01000014;
28      unsigned char *seed_msw_register      = (unsigned char *) 0x01000018;
29      unsigned int *done_register           = (unsigned int *)  0x0100001C;
30      unsigned char *plaintext_address      = (unsigned char *) 0x01000100;
31      unsigned char *cipher_address         = (unsigned char *) 0x01000200;
32    
33      for (i = 0; i < plaintext_len; ++i)
34        plaintext_address[i] = plaintext[i];
35    
36      *plain_address_register = (unsigned int)plaintext_address;
37    
38      *frame_size_register = plaintext_len;
39    
40      *cipher_address_register = (unsigned int)cipher_address;
41    
42      seed_lsw_register[0] = seed[0];
43      seed_lsw_register[1] = seed[1];
44      seed_lsw_register[2] = seed[2];
45      seed_lsw_register[3] = seed[3];
46      seed_msw_register[0] = seed[4];
47      seed_msw_register[1] = seed[5];
48      seed_msw_register[2] = seed[6];
49      seed_msw_register[3] = seed[7];
50    
51      *start_encrypt_register = 1;
52      *start_encrypt_register = 0;
53    
54      while (*done_register != 1)
55        ;
56    
57      for (i = 0; i < plaintext_len; ++i)
58        ciphertext[i] = cipher_address[i];
59    }
60    
61    
62    unsigned long get_crc(int no_of_bytes, unsigned char data[]) {
63      unsigned long crc_temp = 0xFFFFFFFF;
64      int i, index;
65    
66      index = ((int)(crc_temp >> 24)) & 0xFF;
67      crc_temp = (crc_temp << 8) ^ crc_table[index];
68    
69      for (i = 0; i < no_of_bytes; i++) {
70        index = ((int)(crc_temp >> 24) ^ data[i]) & 0xFF;       
71        crc_temp = (crc_temp << 8) ^ crc_table[index];
72      }
73      crc_temp = crc_temp ^ 0xFFFFFFFF;
74    
75      return crc_temp; 
76    }
77    
78    
79    int TestMain() {
80      // ensure that no functions using semihosting SWIs are linked in from the
81      // C library
82    #pragma import(__use_no_semihosting_swi)
83    
84      // Initialize the plaintext data
85      unsigned char plaintext[48] = {
86        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
87        0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
88        0x00, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
89        0x00, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
90        0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89,
91        0x28, 0x47, 0x37, 0x86, 0x31, 0x7F, 0x4D, 0x86 };
92    
93      // Declare the ciphertext array. It has 8 more bytes for 4+4 bytes of CRC data
94      unsigned char ciphertext[48+8]; 
95      unsigned long crc; 
96      int no_of_bytes = 48; 
97      int i; 
98      unsigned char seed[8];
99    
100     // The 5 MSBs of the seed do not change with the frame
101     // The 3 LSBs change with each new frame (part of specification)
102     seed[3] = 0x67;
103     seed[4] = 0x89;
104     seed[5] = 0xAB;
105     seed[6] = 0xCD;
106     seed[7] = 0xEF;
107   
108     // For project-3 we're applying the MAC protocol to only one frame
109     // to have manageable simulation time
110     for (i = 0; i< 1; i++) {
111       seed[0] = 0x01;
112       seed[1] = 0x23;
113       seed[2] = 0x45;
114   
115       // Compute the CRC of the plaintext
116       crc = get_crc(no_of_bytes, plaintext); 
117   
118       // Encrypt the plaintext
119       wep_encrypt(no_of_bytes, plaintext, ciphertext, seed); 
120   
121       // Append the CRC to the ciphertext
122       ciphertext[no_of_bytes]   = (unsigned char)(crc >> 24);
123       ciphertext[no_of_bytes+1] = (unsigned char)(crc >> 16);
124       ciphertext[no_of_bytes+2] = (unsigned char)(crc >> 8);
125       ciphertext[no_of_bytes+3] = (unsigned char)(crc);
126   
127       // Compute the CRC of the whole payload
128       crc = get_crc(no_of_bytes+4, ciphertext);
129   
130       // Append the CRC to the payload
131       ciphertext[no_of_bytes+4]   = (unsigned char)(crc >> 24);
132       ciphertext[no_of_bytes+4+1] = (unsigned char)(crc >> 16);
133       ciphertext[no_of_bytes+4+2] = (unsigned char)(crc >> 8);
134       ciphertext[no_of_bytes+4+3] = (unsigned char)(crc);
135   
136       // Display the final frame which would be transmitted
137       // Cannot have printf statements in code
138       //fprintf(stdout, "%x\n", ciphertext[j]);
139   
140       // Display is handled by XRAY debugger
141       // You'll have to single step through the code for this function
142       // There is some bug in XRAY which causes it to crash if you
143       // just run through this.
144       // Set a breakpoint to just before this function and then single step
145       // through the code to display the final frame which would be transmitted 
146       // Alternately, you can also view the appropriate memory location
147       // of ciphertext to see the final data.
148       WriteDebuggerWindow((unsigned long*) ciphertext, no_of_bytes+8);
149     }
150     return 1;
151   }
