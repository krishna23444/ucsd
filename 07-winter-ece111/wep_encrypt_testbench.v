
module wep_test(); 

reg 	clk; 
reg 	nreset; 
reg 	[31:0] plain_addr; 
reg 	[31:0] cipher_addr; 
reg 	[31:0] frame_size; 
reg 	[31:0] start_encrypt;
reg 	[31:0] seed_msw; 
reg 	[31:0] seed_lsw;
reg     [31:0] port_A_data_out;
reg 	[31:0] dpsram[0:16383];

integer k; 

wire    port_A_clk;
wire    [31:0] port_A_data_in;
wire 	[15:0] port_A_addr;
wire 	port_A_we;
wire 	done; 



wep_encrypt wep_inst (
.clk		(clk),
.nreset		(nreset), 
.start_encrypt	(start_encrypt[0]),
.plain_addr	(plain_addr), 
.frame_size	(frame_size), 
.cipher_addr	(cipher_addr), 
.seed_msw	(seed_msw), 
.seed_lsw	(seed_lsw), 
.done		(done), 
.port_A_clk	(port_A_clk),
.port_A_data_in	(port_A_data_in),
.port_A_data_out(port_A_data_out),
.port_A_addr	(port_A_addr),
.port_A_we	(port_A_we)
);



// CLOCK GENERATOR

always
begin
	#10; 
	clk = 1'b1; 
	#10
	clk = 1'b0; 
end

// MAIN TESTBENCH 

initial
begin

// RESET ENCRYPTION CO-PROCESSOR

	@(posedge clk) nreset = 0; 
	for (k = 0; k < 2; k = k + 1) @(posedge clk);
	nreset = 1; 
	for (k = 0; k < 2; k = k + 1) @(posedge clk);

// READ 100 BYTES FROM FILE  

	$readmemh("plaintext.dat", dpsram);

// DISPLAY PLAINTEXT OF FRAME # 1 (48 BYTES)

	$display("-----------\n"); 
	$display("Plaintext 1\n"); 
	$display("-----------\n"); 
	for (k = 0; k < 12; k = k + 1)
	begin
		$display("%x\n", dpsram[k]); 
	end
			
// SET INPUTS TO ENCRYPTION CO-PROCESSOR

        plain_addr = 32'h0;	
        frame_size = 32'd48; 
        cipher_addr = 32'hC8; 
        seed_msw    = 32'hEFCDAB89; 
        seed_lsw   = 32'h67452301; 
	start_encrypt = 1'b1; 
	for (k = 0; k < 2; k = k + 1) @(posedge clk);
	start_encrypt = 1'b0; 

// WAIT TILL ENTIRE FRAME IS ENCRYPTED, THEN DISPLAY CIPHERTEXT

	wait (done == 1); 

// SOME IDLE CYCLES BEFORE NEXT FRAME 

	for (k = 0; k < 10; k = k + 1) @(posedge clk);

// DISPLAY CIPHERTEXT OF FRAME # 1 (48 BYTES)

	$display("-----------\n"); 
	$display("Ciphertext 1\n"); 
	$display("-----------\n"); 
	for (k = 50; k < 62; k = k + 1)
	begin
		$display("%x\n", dpsram[k]); 
	end

// DISPLAY PLAINTEXT OF FRAME # 2 (51 BYTES)

	$display("-----------\n"); 
        $display("Plaintext 2\n");
	$display("-----------\n"); 
        for (k = 12; k < 25; k = k + 1)
        begin
                $display("%x\n", dpsram[k]);
        end
		$display("Note, most significant byte (00) of last word should not be encrypted\n"); 

// SET INPUTS TO ENCRYPTION CO-PROCESSOR

        plain_addr = 32'h30;
        frame_size = 32'd51;
        cipher_addr = 32'h012C;
        seed_msw    = 32'hEFCDAB89;
        seed_lsw   =  32'h673678AB; // note, last 3 bytes have changed
        start_encrypt = 1'b1;
        for (k = 0; k < 2; k = k + 1) @(posedge clk);
        start_encrypt = 1'b0;

// WAIT TILL ENTIRE FRAME IS ENCRYPTED, THEN DISPLAY CIPHERTEXT

        wait (done == 1);

// DISPLAY CIPHERTEXT OF FRAME # 2 (51 BYTES)

	$display("-----------\n"); 
        $display("Ciphertext 2\n"); 
	$display("-----------\n"); 
        for (k = 75; k < 88; k = k + 1)
        begin
                $display("%x\n", dpsram[k]);
        end
	$stop;
end


// DPSRAM MODEL - (changed on 04/18/03)
// Note (IMPORTANT): 
// 1. The Encryption Block can access the DPSRAM only word-by-word 
//    (and not individual bytes). 

// 2. The timing diagrams for reading and writing memory are given
//    on Project1 webpage. Please refer to them.

always @(posedge port_A_clk)
begin
	if (port_A_addr % 4 == 0)
	begin
		if (port_A_we == 1'b1) 	// write
			dpsram[port_A_addr >> 2] = port_A_data_in;
		else			// read
			port_A_data_out = dpsram[port_A_addr >> 2];
	end
	else
		$display("Error: memory reference not word aligned!\n");
end
endmodule

