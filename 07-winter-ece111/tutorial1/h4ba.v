// 4-bit adder using hierarchical logic.
// A 1-bit full adder is instantiated four times.
// The four 1-bit adders are wired together to comprise a 4-bit adder.

module h4ba (addend_one,addend_two,carry_in,sum,carry_out); 
   input [3:0]   addend_one;   // four bit signal with msb addend_one(3)  
   input [3:0]   addend_two;
   input         carry_in;
   output [3:0]  sum;
   output        carry_out;

   // Declare wires to connect the four instantiations of the full adder together
   // to obtain the four-bit adder.  The carry-out of each of the three lowest bit 
   // adders are each connected to the carry-in of next highest bit adder.
    wire carry_out_0;  // 1-bit wire  
    wire carry_out_1;  // 1-bit wire 
    wire carry_out_2;  // 1-bit wire   

    // 4 instantiations of the full_adder module. 
    // Each instantiation of the full_adder is given a unique name, i.e., fa0.

    full_adder fa0 ( 
                   .a_one(addend_one[0]),
                   .a_two(addend_two[0]),
                   .c_in(carry_in),
                   .s(sum[0]), 
                   .c_out(carry_out_0)
                    );

    full_adder fa1 ( 
                   .a_one(addend_one[1]),
                   .a_two(addend_two[1]),
                   .c_in(carry_out_0),
                   .s(sum[1]), 
                   .c_out(carry_out_1)
                    );

    full_adder fa2 ( 
                   .a_one(addend_one[2]),
                   .a_two(addend_two[2]),
                   .c_in(carry_out_1),
                   .s(sum[2]), 
                   .c_out(carry_out_2)
                    );

    full_adder fa3 ( 
                   .a_one(addend_one[3]),
                   .a_two(addend_two[3]),
                   .c_in(carry_out_2),
                   .s(sum[3]), 
                   .c_out(carry_out)
                    );

endmodule
