module functional_rtl_2 (reset_n, clk, a, b, c, count);
   input reset_n;
   input clk;
   input [4:0] a;
   input [4:0] b;
   output [5:0] c;
   output [5:0] count;

   reg [5:0] c;               // Signals written to in an always block are declared as type reg.
   reg [1:0] current_state;   // Will synthesize to ffs only if the always block 
   reg [1:0] next_state;      // is sensitive to an edge, i.e. posedge clk.
   reg [5:0] count;
   reg [5:0] next_count;
   reg [5:0] next_c;          // Number of bits is 6 with the msb on the left: next_c(5).

   parameter  s0=2'b00,    // Declaration of states using a parameter (constant) declaration.
              s1=2'b01,
              s2=2'b10;    // Number of bits is 2; the value in binary is 10. 


   always @ (current_state or a or b or count)   //unclocked always block for combinational elements.
   case (current_state)
      s0: begin
         next_count = 6'b000000;    // Number of bits is 6; the value in binary is 000000.
         next_c     = 6'd0;         // Number of bits is 6; the value in decimal is 0.      
         next_state = s1;            
      end
      s1: begin
         next_count = (count + 1) % 64;         // % is the modulo operator.
         next_c = a + b;
         next_state = s2;
      end
      s2: begin
	     next_count = (count + 1) % 64;
	     next_c = a - b;
	     next_state = s0;
	  end
      default: begin                   // Unclocked always block: default state so that  
         next_count = 6'b000000;       // the case statement is full and latches are not  
         next_c     = 6'b000000;       // synthesized.  Signals written to in the always   
         next_state = s0;              // block must be assigned a value for any possible 
      end                              // path through the always block.
   endcase

   always@(posedge clk or negedge reset_n)
   if (!reset_n)                               // active low asynchronous reset.
      begin                                    // ! is the logical negation operator.
	 count <= 6'b000000;
	 c <= 6'b000000;
	 current_state <= s0;
      end
   else
      begin
         count <= next_count;
         c <= next_c;
         current_state <= next_state;
       end

endmodule
