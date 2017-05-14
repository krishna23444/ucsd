module reg32(reset_n,clk,D,Q);

   input reset_n;
   input clk;
   input [31:0] D;
   output [31:0] Q;

   reg [31:0] Q;

   always @ (negedge reset_n or posedge clk)
   begin
      if (!reset_n) 
         Q <= 32'd0;
      else
         Q <= D;
   end

endmodule
   
