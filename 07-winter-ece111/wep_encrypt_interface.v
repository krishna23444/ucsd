module wep_encrypt (       
	clk, 		
	nreset, 	
	start_encrypt,  
	plain_addr,	
	frame_size, 	
	cipher_addr, 	
	seed_msw, 
	seed_lsw, 
	done, 		
	port_A_clk,
        port_A_data_in,
        port_A_data_out,
        port_A_addr,
        port_A_we
	);

input	clk;
input	nreset; 
// Initializes the wep_encrypt module

input	start_encrypt; 
// Tells wep_encrypt to start encrypting the given frame

input 	[31:0] plain_addr; 
// Starting address of the plaintext frame
// i.e., specifies from where wep_encrypt must read the plaintext frame

input	[31:0] frame_size; 
// Length of the frame in bytes

input	[31:0] cipher_addr; 
// Starting address of the ciphertext frame
// i.e., specifies where wep_encrypt must write the ciphertext frame

input 	[31:0] seed_msw; 
// Contains the 4 most significant bytes of the 64 bit seed

input 	[31:0] seed_lsw; 
// Contains the 4 least significant bytes of the 64 bit seed

input   [31:0] port_A_data_out; 
// read data from the dpsram (plaintext)

output  [31:0] port_A_data_in;
// write data to the dpsram (ciphertext)

output  [15:0] port_A_addr;
// address of dpsram being read/written 

output  port_A_clk;
// clock to dpsram (drive this with the input clk) 

output  port_A_we;
// read/write selector for dpsram

output	done; // done is a signal to indicate that encryption of the frame is complete
