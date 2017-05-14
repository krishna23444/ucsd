// Verilog behavioral description of a 4-bit adder
module process_4_bit_adder(addend_one, addend_two, carry_in, sum, carry_out);
   input [3:0] addend_one;
   input [3:0] addend_two;
   input carry_in;
   output [3:0] sum;
   output carry_out;

   reg [3:0] sum;    // Declare as type reg since assigned to in the always block.
   reg carry_out;
   reg [4:0] carry;
   integer i;

   always@(addend_one or addend_two or carry_in)    // sensitivity list
   begin: add_4_bit_process
   carry[0] = carry_in;
   for (i=0;i<=3; i=i+1)  
      begin
      sum[i] = addend_one[i] ^ addend_two[i] ^ carry[i];
      carry[i+1] = addend_one[i] & addend_two[i] | 
                   addend_one[i] & carry[i] |
                   addend_two[i] & carry[i];
      end
   carry_out = carry[4];
   end
endmodule
