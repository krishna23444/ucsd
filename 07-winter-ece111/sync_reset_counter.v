// Verilog version of up counter with synchronous reset
module sync_reset_counter(clk,reset,q);
input clk;
input reset;
output [4:0] q;
reg [4:0] current_state;
reg [4:0] next_state;

   always @ (reset or current_state) 
   begin
      if (reset==1)
               next_state = 0;  // sync_reset it back to all 0's
      else
         next_state = incr(current_state);
   end   // always

   assign q = current_state;    // feeding the counter value to output 

   always @ (posedge clk)
   begin
      current_state <= next_state;  // process (always block) to represent
   end    // always                 // the 5-bit register

   function [4:0] incr;            // function to increment by 1 
   input [4:0] n;
	 reg  c;
         integer i;
   begin    
      c=1;
      for (i = 0; i<=4; i=i+1)
	 begin
	 incr[i] = n[i] ^ c;
	 c = n[i] & c;
	 end
   end
   endfunction

endmodule
