module combinational_loop (A,B,Y,W); 
   input A,B;
   output Y,W;
   reg Y,W;

   always@( A or W) Y = A | W;

   always@(B or Y) W = B & Y;

endmodule
