module clocked_loop (A,B,CLK,RESET_N,Y,W); 
   input  A,B,CLK,RESET_N;
   output Y,W; 
   reg Y,W;

   always@(posedge CLK or negedge RESET_N) 
   begin
      if (! RESET_N)
         Y <= 1'b0;
      else
         Y <= A | W;
   end

   always@(B or Y) W = B & Y;

endmodule
