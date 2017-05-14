module wep_encrypt (clk, nreset, 
                    start_encrypt, plain_addr, frame_size, cipher_addr,   
                    seed_msw, seed_lsw, done,
                    port_A_clk, port_A_data_in, port_A_data_out,
                    port_A_addr, port_A_we
                   );

  input clk;
  input nreset; 
  // Initializes the wep_encrypt_v3 module

//  input start_encrypt; 
  input [31:0] start_encrypt; 
  // Tells wep_encrypt_v3 to start encrypting the given frame

  input [31:0] plain_addr; 
  // Starting address of the plaintext frame
  // i.e., specifies from where wep_encrypt_v3 must read the plaintext frame

  input [31:0] frame_size; 
  // Length of the frame in bytes

  input [31:0] cipher_addr; 
  // Starting address of the ciphertext frame
  // i.e., specifies where wep_encrypt_v3 must write the ciphertext frame

  input [31:0] seed_msw; 
  // Contains the 4 most significant bytes of the 64 bit seed

  input [31:0] seed_lsw; 
  // Contains the 4 least significant bytes of the 64 bit seed

  input [31:0] port_A_data_out; 
  // read data from the dpsram (plaintext)

  output [31:0] port_A_data_in;
  // write data to the dpsram (ciphertext)

//  output [15:0] port_A_addr;
  output [31:0] port_A_addr;
  // address of dpsram being read/written 

  output port_A_clk;
  // clock to dpsram (drive this with the input clk) 

  output port_A_we;
  // read/write selector for dpsram

  output done;
  // done is a signal to indicate that encryption of the frame is complete

  wire [31:0] start_encrypt;
  wire [31:0] plain_addr;
  wire [31:0] cipher_addr;
  wire [31:0] frame_size;
  wire [31:0] seed_lsw;
  wire [31:0] seed_msw;
  wire [31:0] port_A_data_out;

  reg [31:0] port_A_data_in;
  reg done;
//  reg [15:0] port_A_addr;
  reg [31:0] port_A_addr;
  reg port_A_clk;
  reg port_A_we;

  reg [7:0] sbox [0:255];
  reg [7:0] seed [0:7];
  reg [3:0] current_state;
  reg [31:0] count;
  reg [1:0] byte_count;
  reg [7:0] i;
  reg [7:0] j;
  reg [7:0] index;

  parameter s0  = 4'b0000,
            s1  = 4'b0001,
            s2  = 4'b0010,
            s3  = 4'b0011,
            s4  = 4'b0100,
            s5  = 4'b0101,
            s6  = 4'b0110,
            s7  = 4'b0111,
            s8  = 4'b1000,
            s9  = 4'b1001,
            s10 = 4'b1010,
            s11 = 4'b1011,
            s12 = 4'b1100,
            s13 = 4'b1101,
            s14 = 4'b1110,
            s15 = 4'b1111;

  always@ (clk) begin
    port_A_clk <= clk;
  end

  always@ (posedge clk) begin
    if (nreset == 1'b0)
      current_state <= s0;
    else begin
      case (current_state)
        // start
        s0: begin
          done <= 1'b0;
          count <= 32'h0;
          i <= 8'h0;
          j <= seed_lsw[7:0];
          port_A_addr <= 32'h0;

          sbox[0] <= 8'h0;
          sbox[1] <= 8'h1;
          sbox[2] <= 8'h2;
          sbox[3] <= 8'h3;
          sbox[4] <= 8'h4;
          sbox[5] <= 8'h5;
          sbox[6] <= 8'h6;
          sbox[7] <= 8'h7;
          sbox[8] <= 8'h8;
          sbox[9] <= 8'h9;
          sbox[10] <= 8'ha;
          sbox[11] <= 8'hb;
          sbox[12] <= 8'hc;
          sbox[13] <= 8'hd;
          sbox[14] <= 8'he;
          sbox[15] <= 8'hf;
          sbox[16] <= 8'h10;
          sbox[17] <= 8'h11;
          sbox[18] <= 8'h12;
          sbox[19] <= 8'h13;
          sbox[20] <= 8'h14;
          sbox[21] <= 8'h15;
          sbox[22] <= 8'h16;
          sbox[23] <= 8'h17;
          sbox[24] <= 8'h18;
          sbox[25] <= 8'h19;
          sbox[26] <= 8'h1a;
          sbox[27] <= 8'h1b;
          sbox[28] <= 8'h1c;
          sbox[29] <= 8'h1d;
          sbox[30] <= 8'h1e;
          sbox[31] <= 8'h1f;
          sbox[32] <= 8'h20;
          sbox[33] <= 8'h21;
          sbox[34] <= 8'h22;
          sbox[35] <= 8'h23;
          sbox[36] <= 8'h24;
          sbox[37] <= 8'h25;
          sbox[38] <= 8'h26;
          sbox[39] <= 8'h27;
          sbox[40] <= 8'h28;
          sbox[41] <= 8'h29;
          sbox[42] <= 8'h2a;
          sbox[43] <= 8'h2b;
          sbox[44] <= 8'h2c;
          sbox[45] <= 8'h2d;
          sbox[46] <= 8'h2e;
          sbox[47] <= 8'h2f;
          sbox[48] <= 8'h30;
          sbox[49] <= 8'h31;
          sbox[50] <= 8'h32;
          sbox[51] <= 8'h33;
          sbox[52] <= 8'h34;
          sbox[53] <= 8'h35;
          sbox[54] <= 8'h36;
          sbox[55] <= 8'h37;
          sbox[56] <= 8'h38;
          sbox[57] <= 8'h39;
          sbox[58] <= 8'h3a;
          sbox[59] <= 8'h3b;
          sbox[60] <= 8'h3c;
          sbox[61] <= 8'h3d;
          sbox[62] <= 8'h3e;
          sbox[63] <= 8'h3f;
          sbox[64] <= 8'h40;
          sbox[65] <= 8'h41;
          sbox[66] <= 8'h42;
          sbox[67] <= 8'h43;
          sbox[68] <= 8'h44;
          sbox[69] <= 8'h45;
          sbox[70] <= 8'h46;
          sbox[71] <= 8'h47;
          sbox[72] <= 8'h48;
          sbox[73] <= 8'h49;
          sbox[74] <= 8'h4a;
          sbox[75] <= 8'h4b;
          sbox[76] <= 8'h4c;
          sbox[77] <= 8'h4d;
          sbox[78] <= 8'h4e;
          sbox[79] <= 8'h4f;
          sbox[80] <= 8'h50;
          sbox[81] <= 8'h51;
          sbox[82] <= 8'h52;
          sbox[83] <= 8'h53;
          sbox[84] <= 8'h54;
          sbox[85] <= 8'h55;
          sbox[86] <= 8'h56;
          sbox[87] <= 8'h57;
          sbox[88] <= 8'h58;
          sbox[89] <= 8'h59;
          sbox[90] <= 8'h5a;
          sbox[91] <= 8'h5b;
          sbox[92] <= 8'h5c;
          sbox[93] <= 8'h5d;
          sbox[94] <= 8'h5e;
          sbox[95] <= 8'h5f;
          sbox[96] <= 8'h60;
          sbox[97] <= 8'h61;
          sbox[98] <= 8'h62;
          sbox[99] <= 8'h63;
          sbox[100] <= 8'h64;
          sbox[101] <= 8'h65;
          sbox[102] <= 8'h66;
          sbox[103] <= 8'h67;
          sbox[104] <= 8'h68;
          sbox[105] <= 8'h69;
          sbox[106] <= 8'h6a;
          sbox[107] <= 8'h6b;
          sbox[108] <= 8'h6c;
          sbox[109] <= 8'h6d;
          sbox[110] <= 8'h6e;
          sbox[111] <= 8'h6f;
          sbox[112] <= 8'h70;
          sbox[113] <= 8'h71;
          sbox[114] <= 8'h72;
          sbox[115] <= 8'h73;
          sbox[116] <= 8'h74;
          sbox[117] <= 8'h75;
          sbox[118] <= 8'h76;
          sbox[119] <= 8'h77;
          sbox[120] <= 8'h78;
          sbox[121] <= 8'h79;
          sbox[122] <= 8'h7a;
          sbox[123] <= 8'h7b;
          sbox[124] <= 8'h7c;
          sbox[125] <= 8'h7d;
          sbox[126] <= 8'h7e;
          sbox[127] <= 8'h7f;
          sbox[128] <= 8'h80;
          sbox[129] <= 8'h81;
          sbox[130] <= 8'h82;
          sbox[131] <= 8'h83;
          sbox[132] <= 8'h84;
          sbox[133] <= 8'h85;
          sbox[134] <= 8'h86;
          sbox[135] <= 8'h87;
          sbox[136] <= 8'h88;
          sbox[137] <= 8'h89;
          sbox[138] <= 8'h8a;
          sbox[139] <= 8'h8b;
          sbox[140] <= 8'h8c;
          sbox[141] <= 8'h8d;
          sbox[142] <= 8'h8e;
          sbox[143] <= 8'h8f;
          sbox[144] <= 8'h90;
          sbox[145] <= 8'h91;
          sbox[146] <= 8'h92;
          sbox[147] <= 8'h93;
          sbox[148] <= 8'h94;
          sbox[149] <= 8'h95;
          sbox[150] <= 8'h96;
          sbox[151] <= 8'h97;
          sbox[152] <= 8'h98;
          sbox[153] <= 8'h99;
          sbox[154] <= 8'h9a;
          sbox[155] <= 8'h9b;
          sbox[156] <= 8'h9c;
          sbox[157] <= 8'h9d;
          sbox[158] <= 8'h9e;
          sbox[159] <= 8'h9f;
          sbox[160] <= 8'ha0;
          sbox[161] <= 8'ha1;
          sbox[162] <= 8'ha2;
          sbox[163] <= 8'ha3;
          sbox[164] <= 8'ha4;
          sbox[165] <= 8'ha5;
          sbox[166] <= 8'ha6;
          sbox[167] <= 8'ha7;
          sbox[168] <= 8'ha8;
          sbox[169] <= 8'ha9;
          sbox[170] <= 8'haa;
          sbox[171] <= 8'hab;
          sbox[172] <= 8'hac;
          sbox[173] <= 8'had;
          sbox[174] <= 8'hae;
          sbox[175] <= 8'haf;
          sbox[176] <= 8'hb0;
          sbox[177] <= 8'hb1;
          sbox[178] <= 8'hb2;
          sbox[179] <= 8'hb3;
          sbox[180] <= 8'hb4;
          sbox[181] <= 8'hb5;
          sbox[182] <= 8'hb6;
          sbox[183] <= 8'hb7;
          sbox[184] <= 8'hb8;
          sbox[185] <= 8'hb9;
          sbox[186] <= 8'hba;
          sbox[187] <= 8'hbb;
          sbox[188] <= 8'hbc;
          sbox[189] <= 8'hbd;
          sbox[190] <= 8'hbe;
          sbox[191] <= 8'hbf;
          sbox[192] <= 8'hc0;
          sbox[193] <= 8'hc1;
          sbox[194] <= 8'hc2;
          sbox[195] <= 8'hc3;
          sbox[196] <= 8'hc4;
          sbox[197] <= 8'hc5;
          sbox[198] <= 8'hc6;
          sbox[199] <= 8'hc7;
          sbox[200] <= 8'hc8;
          sbox[201] <= 8'hc9;
          sbox[202] <= 8'hca;
          sbox[203] <= 8'hcb;
          sbox[204] <= 8'hcc;
          sbox[205] <= 8'hcd;
          sbox[206] <= 8'hce;
          sbox[207] <= 8'hcf;
          sbox[208] <= 8'hd0;
          sbox[209] <= 8'hd1;
          sbox[210] <= 8'hd2;
          sbox[211] <= 8'hd3;
          sbox[212] <= 8'hd4;
          sbox[213] <= 8'hd5;
          sbox[214] <= 8'hd6;
          sbox[215] <= 8'hd7;
          sbox[216] <= 8'hd8;
          sbox[217] <= 8'hd9;
          sbox[218] <= 8'hda;
          sbox[219] <= 8'hdb;
          sbox[220] <= 8'hdc;
          sbox[221] <= 8'hdd;
          sbox[222] <= 8'hde;
          sbox[223] <= 8'hdf;
          sbox[224] <= 8'he0;
          sbox[225] <= 8'he1;
          sbox[226] <= 8'he2;
          sbox[227] <= 8'he3;
          sbox[228] <= 8'he4;
          sbox[229] <= 8'he5;
          sbox[230] <= 8'he6;
          sbox[231] <= 8'he7;
          sbox[232] <= 8'he8;
          sbox[233] <= 8'he9;
          sbox[234] <= 8'hea;
          sbox[235] <= 8'heb;
          sbox[236] <= 8'hec;
          sbox[237] <= 8'hed;
          sbox[238] <= 8'hee;
          sbox[239] <= 8'hef;
          sbox[240] <= 8'hf0;
          sbox[241] <= 8'hf1;
          sbox[242] <= 8'hf2;
          sbox[243] <= 8'hf3;
          sbox[244] <= 8'hf4;
          sbox[245] <= 8'hf5;
          sbox[246] <= 8'hf6;
          sbox[247] <= 8'hf7;
          sbox[248] <= 8'hf8;
          sbox[249] <= 8'hf9;
          sbox[250] <= 8'hfa;
          sbox[251] <= 8'hfb;
          sbox[252] <= 8'hfc;
          sbox[253] <= 8'hfd;
          sbox[254] <= 8'hfe;
          sbox[255] <= 8'hff;

          seed[0] <= seed_lsw[7:0];
          seed[1] <= seed_lsw[15:8];
          seed[2] <= seed_lsw[23:16];
          seed[3] <= seed_lsw[31:24];
          seed[4] <= seed_msw[7:0];
          seed[5] <= seed_msw[15:8];
          seed[6] <= seed_msw[23:16];
          seed[7] <= seed_msw[31:24];
          if (start_encrypt & 32'b1 == 1)
            current_state <= s1;
        end

        // construct sbox
        s1: begin
          sbox[i] <= sbox[j];
          sbox[j] <= sbox[i];
          i <= i + 8'h1;

          if (i == 8'hff)
            current_state <= s3;
          else
            current_state <= s2;
        end

        // compute j
        s2: begin
          j <= (j + sbox[i] + seed[i % 8]) % 256;

          current_state <= s1;
        end

        // initialize encryption
        s3: begin
          i <= 8'h1;
          j <= sbox[1] % 256;

          current_state <= s9;
        end

        // delay for writing of last word
        s9: begin
          current_state <= s4;
        end

        // read word
        s4: begin
          port_A_we <= 8'h0;
          port_A_addr <= plain_addr + count;
          byte_count <= 2'b0;

          if (count >= frame_size)
            current_state <= s8;
          else
            current_state <= s5;
        end

        // swap
        s5: begin
          sbox[i] <= sbox[j];
          sbox[j] <= sbox[i];
          index <= (sbox[i] + sbox[j]) % 256;
          i <= (i + 1) % 256;

          if (count + byte_count >= frame_size)
            current_state <= s7;
          else
            current_state <= s6;
        end

        // encrypt byte
        s6: begin
          byte_count <= byte_count + 1;
          case (byte_count)
            2'b00: begin
              port_A_data_in[7:0] <= sbox[index] ^ port_A_data_out[7:0];
              port_A_data_in[31:8] <= port_A_data_out[31:8];
            end
            2'b01: begin
              port_A_data_in[15:8] <= sbox[index] ^ port_A_data_out[15:8];
            end
            2'b10: begin
              port_A_data_in[23:16] <= sbox[index] ^ port_A_data_out[23:16];
            end
            2'b11: begin
              port_A_data_in[31:24] <= sbox[index] ^ port_A_data_out[31:24];
            end
          endcase

          j <= (j + sbox[i]) % 256;

          if (byte_count == 2'b11)
            current_state <= s7;
          else
            current_state <= s5;
        end

        // write word
        s7: begin
          port_A_we <= 1'b1;
          port_A_addr <= cipher_addr + count;
          count <= count + 4;

          current_state <= s9;
        end

        // done
        s8: begin
          done <= 1'b1;

          current_state <=  s0;
        end

        default: begin
          current_state <= s0;
        end
      endcase
    end
  end
endmodule
