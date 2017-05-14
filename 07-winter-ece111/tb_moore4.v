
module tb_moore4;    // Verilog tb for moore4 state machine    
   reg reset_n;
   reg clk;
   wire z;
   integer i;

   moore4 uut (                        
               .RESET_N(reset_n),     // instantiation of state machine
               .CLK(clk),        
               .Z(z)
              );
   
   initial
      begin
         clk = 0;
         forever #10 clk = ~clk;      // create clock
      end

   initial  
      begin
         reset_n = 0;
         @ (negedge clk);
         @ (negedge clk);
         reset_n = 1;
         for (i=0; i<8; i=i+1)
            @ (negedge clk);
         $stop;
      end 

endmodule
