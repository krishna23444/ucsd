module wep_encrypt_v3 (clk, nreset, 
                       start_encrypt, plain_addr, frame_size, cipher_addr, 	
	                     seed_msw, seed_lsw, done,
                       port_A_clk, port_A_data_in, port_A_data_out,
                       port_A_addr, port_A_we
	                    );

  input	clk;
  input	nreset; 
  // Initializes the wep_encrypt_v3 module

  input	start_encrypt; 
  // Tells wep_encrypt_v3 to start encrypting the given frame

  input 	[31:0] plain_addr; 
  // Starting address of the plaintext frame
  // i.e., specifies from where wep_encrypt_v3 must read the plaintext frame

  input	[31:0] frame_size; 
  // Length of the frame in bytes

  input	[31:0] cipher_addr; 
  // Starting address of the ciphertext frame
  // i.e., specifies where wep_encrypt_v3 must write the ciphertext frame

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

  wire start_encrypt;
  wire [31:0] plain_addr;
  wire [31:0] cipher_addr;
  wire [31:0] frame_size;
  wire [31:0] seed_lsw;
  wire [31:0] seed_msw;
  wire [31:0] port_A_data_out;

  reg [31:0] port_A_data_in;
  reg done;
  reg [15:0] port_A_addr;
  reg port_A_clk;
  reg port_A_we;

  reg [7:0] sbox [0:255];
  reg [7:0] seed [0:7];
  reg [3:0] current_state;
  reg [31:0] count;
  reg [31:0] byte_count;
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
        s0: begin
              done <= 0;
              count <= 0;
              i <= 0;
              j <= seed_lsw[7:0];

              sbox[0] <= 0;
              sbox[1] <= 1;
              sbox[2] <= 2;
              sbox[3] <= 3;
              sbox[4] <= 4;
              sbox[5] <= 5;
              sbox[6] <= 6;
              sbox[7] <= 7;
              sbox[8] <= 8;
              sbox[9] <= 9;
              sbox[10] <= 10;
              sbox[11] <= 11;
              sbox[12] <= 12;
              sbox[13] <= 13;
              sbox[14] <= 14;
              sbox[15] <= 15;
              sbox[16] <= 16;
              sbox[17] <= 17;
              sbox[18] <= 18;
              sbox[19] <= 19;
              sbox[20] <= 20;
              sbox[21] <= 21;
              sbox[22] <= 22;
              sbox[23] <= 23;
              sbox[24] <= 24;
              sbox[25] <= 25;
              sbox[26] <= 26;
              sbox[27] <= 27;
              sbox[28] <= 28;
              sbox[29] <= 29;
              sbox[30] <= 30;
              sbox[31] <= 31;
              sbox[32] <= 32;
              sbox[33] <= 33;
              sbox[34] <= 34;
              sbox[35] <= 35;
              sbox[36] <= 36;
              sbox[37] <= 37;
              sbox[38] <= 38;
              sbox[39] <= 39;
              sbox[40] <= 40;
              sbox[41] <= 41;
              sbox[42] <= 42;
              sbox[43] <= 43;
              sbox[44] <= 44;
              sbox[45] <= 45;
              sbox[46] <= 46;
              sbox[47] <= 47;
              sbox[48] <= 48;
              sbox[49] <= 49;
              sbox[50] <= 50;
              sbox[51] <= 51;
              sbox[52] <= 52;
              sbox[53] <= 53;
              sbox[54] <= 54;
              sbox[55] <= 55;
              sbox[56] <= 56;
              sbox[57] <= 57;
              sbox[58] <= 58;
              sbox[59] <= 59;
              sbox[60] <= 60;
              sbox[61] <= 61;
              sbox[62] <= 62;
              sbox[63] <= 63;
              sbox[64] <= 64;
              sbox[65] <= 65;
              sbox[66] <= 66;
              sbox[67] <= 67;
              sbox[68] <= 68;
              sbox[69] <= 69;
              sbox[70] <= 70;
              sbox[71] <= 71;
              sbox[72] <= 72;
              sbox[73] <= 73;
              sbox[74] <= 74;
              sbox[75] <= 75;
              sbox[76] <= 76;
              sbox[77] <= 77;
              sbox[78] <= 78;
              sbox[79] <= 79;
              sbox[80] <= 80;
              sbox[81] <= 81;
              sbox[82] <= 82;
              sbox[83] <= 83;
              sbox[84] <= 84;
              sbox[85] <= 85;
              sbox[86] <= 86;
              sbox[87] <= 87;
              sbox[88] <= 88;
              sbox[89] <= 89;
              sbox[90] <= 90;
              sbox[91] <= 91;
              sbox[92] <= 92;
              sbox[93] <= 93;
              sbox[94] <= 94;
              sbox[95] <= 95;
              sbox[96] <= 96;
              sbox[97] <= 97;
              sbox[98] <= 98;
              sbox[99] <= 99;
              sbox[100] <= 100;
              sbox[101] <= 101;
              sbox[102] <= 102;
              sbox[103] <= 103;
              sbox[104] <= 104;
              sbox[105] <= 105;
              sbox[106] <= 106;
              sbox[107] <= 107;
              sbox[108] <= 108;
              sbox[109] <= 109;
              sbox[110] <= 110;
              sbox[111] <= 111;
              sbox[112] <= 112;
              sbox[113] <= 113;
              sbox[114] <= 114;
              sbox[115] <= 115;
              sbox[116] <= 116;
              sbox[117] <= 117;
              sbox[118] <= 118;
              sbox[119] <= 119;
              sbox[120] <= 120;
              sbox[121] <= 121;
              sbox[122] <= 122;
              sbox[123] <= 123;
              sbox[124] <= 124;
              sbox[125] <= 125;
              sbox[126] <= 126;
              sbox[127] <= 127;
              sbox[128] <= 128;
              sbox[129] <= 129;
              sbox[130] <= 130;
              sbox[131] <= 131;
              sbox[132] <= 132;
              sbox[133] <= 133;
              sbox[134] <= 134;
              sbox[135] <= 135;
              sbox[136] <= 136;
              sbox[137] <= 137;
              sbox[138] <= 138;
              sbox[139] <= 139;
              sbox[140] <= 140;
              sbox[141] <= 141;
              sbox[142] <= 142;
              sbox[143] <= 143;
              sbox[144] <= 144;
              sbox[145] <= 145;
              sbox[146] <= 146;
              sbox[147] <= 147;
              sbox[148] <= 148;
              sbox[149] <= 149;
              sbox[150] <= 150;
              sbox[151] <= 151;
              sbox[152] <= 152;
              sbox[153] <= 153;
              sbox[154] <= 154;
              sbox[155] <= 155;
              sbox[156] <= 156;
              sbox[157] <= 157;
              sbox[158] <= 158;
              sbox[159] <= 159;
              sbox[160] <= 160;
              sbox[161] <= 161;
              sbox[162] <= 162;
              sbox[163] <= 163;
              sbox[164] <= 164;
              sbox[165] <= 165;
              sbox[166] <= 166;
              sbox[167] <= 167;
              sbox[168] <= 168;
              sbox[169] <= 169;
              sbox[170] <= 170;
              sbox[171] <= 171;
              sbox[172] <= 172;
              sbox[173] <= 173;
              sbox[174] <= 174;
              sbox[175] <= 175;
              sbox[176] <= 176;
              sbox[177] <= 177;
              sbox[178] <= 178;
              sbox[179] <= 179;
              sbox[180] <= 180;
              sbox[181] <= 181;
              sbox[182] <= 182;
              sbox[183] <= 183;
              sbox[184] <= 184;
              sbox[185] <= 185;
              sbox[186] <= 186;
              sbox[187] <= 187;
              sbox[188] <= 188;
              sbox[189] <= 189;
              sbox[190] <= 190;
              sbox[191] <= 191;
              sbox[192] <= 192;
              sbox[193] <= 193;
              sbox[194] <= 194;
              sbox[195] <= 195;
              sbox[196] <= 196;
              sbox[197] <= 197;
              sbox[198] <= 198;
              sbox[199] <= 199;
              sbox[200] <= 200;
              sbox[201] <= 201;
              sbox[202] <= 202;
              sbox[203] <= 203;
              sbox[204] <= 204;
              sbox[205] <= 205;
              sbox[206] <= 206;
              sbox[207] <= 207;
              sbox[208] <= 208;
              sbox[209] <= 209;
              sbox[210] <= 210;
              sbox[211] <= 211;
              sbox[212] <= 212;
              sbox[213] <= 213;
              sbox[214] <= 214;
              sbox[215] <= 215;
              sbox[216] <= 216;
              sbox[217] <= 217;
              sbox[218] <= 218;
              sbox[219] <= 219;
              sbox[220] <= 220;
              sbox[221] <= 221;
              sbox[222] <= 222;
              sbox[223] <= 223;
              sbox[224] <= 224;
              sbox[225] <= 225;
              sbox[226] <= 226;
              sbox[227] <= 227;
              sbox[228] <= 228;
              sbox[229] <= 229;
              sbox[230] <= 230;
              sbox[231] <= 231;
              sbox[232] <= 232;
              sbox[233] <= 233;
              sbox[234] <= 234;
              sbox[235] <= 235;
              sbox[236] <= 236;
              sbox[237] <= 237;
              sbox[238] <= 238;
              sbox[239] <= 239;
              sbox[240] <= 240;
              sbox[241] <= 241;
              sbox[242] <= 242;
              sbox[243] <= 243;
              sbox[244] <= 244;
              sbox[245] <= 245;
              sbox[246] <= 246;
              sbox[247] <= 247;
              sbox[248] <= 248;
              sbox[249] <= 249;
              sbox[250] <= 250;
              sbox[251] <= 251;
              sbox[252] <= 252;
              sbox[253] <= 253;
              sbox[254] <= 254;
              sbox[255] <= 255;

              seed[0] <= seed_lsw[7:0];
              seed[1] <= seed_lsw[15:8];
              seed[2] <= seed_lsw[23:16];
              seed[3] <= seed_lsw[31:24];
              seed[4] <= seed_msw[7:0];
              seed[5] <= seed_msw[15:8];
              seed[6] <= seed_msw[23:16];
              seed[7] <= seed_msw[31:24];
              if (start_encrypt == 1)
                current_state <= s1;
            end

        // Construct sbox
        s1: begin
              sbox[i] <= sbox[j];
              sbox[j] <= sbox[i];
              i <= i + 1;

              j <= (j + sbox[i+1] + seed[(i+1)%8]) % 256;

              if (i == 255)
                current_state <= s10;
              else if (frame_size == 0)
                current_state <= s8;
            end

/*
        s9: begin
              sbox[i] <= sbox[j];
              sbox[j] <= sbox[i];
              i <= i + 1;

              current_state <= s1;
            end
*/
        // Initialize Encryption
        s10: begin

              i <= 1;
              j <= sbox[1] % 256;

              current_state <= s2;
            end

        // read word
        s2: begin
              port_A_we <= 0;
              port_A_addr <= plain_addr + count;
              sbox[i] <= sbox[j];
              sbox[j] <= sbox[i];

              index <= (sbox[i] + sbox[j]) % 256;
              
              i <= (i + 1) % 256;
              j <= (j + sbox[(i+1) % 256]) % 256;

              count <= count + 1;

              current_state <= s3;
            end

        // encrypt first byte
        s3: begin
              port_A_data_in[7:0] <= sbox[index] ^ port_A_data_out[7:0];
              port_A_data_in[31:8] <= port_A_data_out[31:8];

              sbox[i] <= sbox[j];
              sbox[j] <= sbox[i];

              index <= (sbox[i] + sbox[j]) % 256;
              i <= (i + 1) % 256;
              j <= (j + sbox[(i+1) % 256]) % 256;

              count <= count + 1;

              if (count >= frame_size)
                current_state <= s7;
              else
                current_state <= s4;
            end

        // encrypt second byte
        s4: begin
              port_A_data_in[15:8] <= sbox[index] ^ port_A_data_out[15:8];

              sbox[i] <= sbox[j];
              sbox[j] <= sbox[i];

              index <= (sbox[i] + sbox[j]) % 256;
              i <= (i + 1) % 256;
              j <= (j + sbox[(i+1) % 256]) % 256;

              count <= count + 1;

              if (count >= frame_size)
                current_state <= s7;
              else
                current_state <= s5;
            end

        // encrypt third byte
        s5: begin
              port_A_data_in[23:16] <= sbox[index] ^ port_A_data_out[23:16];

              sbox[i] <= sbox[j];
              sbox[j] <= sbox[i];

              index <= (sbox[i] + sbox[j]) % 256;
              i <= (i + 1) % 256;
              j <= (j + sbox[(i+1) % 256]) % 256;

              count <= count + 1;

              if (count >= frame_size)
                current_state <= s7;
              else
                current_state <= s6;
            end

        // encrypt fourth byte
        s6: begin
              port_A_data_in[31:24] <= sbox[index] ^ port_A_data_out[31:24];

              current_state <= s7;
            end

        // Write word
        s7: begin
              port_A_we <= 1;
              port_A_addr <= cipher_addr + ((count - 1) & 32'hFFFFFFFC);

              if (count >= frame_size)
                current_state <= s8;
              else
                current_state <= s2;

            end

        s8: begin
              done <= 1;

              current_state <=  s0;
            end

        default:
            current_state <= s0;
      endcase
    end
  end
endmodule
