module functional_rtl (reset_n, clk, a, b, c, count);
   input reset_n;
   input clk;
   input [4:0] a;
   input [4:0] b;
   output [5:0] c;
   output [5:0] count;     // Number of bits is 6 with the msb on the left: count(5)

   reg [5:0] c;      // Signals written to in an always block are declared as type reg.
   reg [1:0] state;  // Will synthesize to ffs only if the always
   reg [5:0] count;  // block is sensitive to an edge, i.e., posedge clk.

   parameter  s0=0,  // Declaration of states using a parameter (constant) declaration.
	      s1=1,
	      s2=2;

   always @ (posedge clk or negedge reset_n)     // clocked process (always block)
   if (! reset_n)                                // active low asynchronous reset.
       begin                                     // ! is the logical negation operator.
	  count <= 6'b000000;       // Number of bits is 6; the value in binary is 000000.
	  c <= 6'b000000;           // Need the begin,end keyword pair if there are multiple
	  state <= s0;              // statements in a branch.
       end
   else
       begin
	  case (state)
	     s0: begin
		    count <= 6'b000000;
		    c <= 6'b000000;      // Outputs  count,c are registered.
		    state <= s1;         // state is synthesized as the state register.
                 end
             s1: begin
		    c <= a + b;
		    count <= (count + 1) % 64;   // % is the modulo operator
		    state <= s2;                 
		 end
	     s2: begin
		    c <= a - b;
		    count <= (count + 1) % 64;
		    state <= s0;
		 end
	  endcase
       end

endmodule
