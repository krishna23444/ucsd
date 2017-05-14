
module tb_moore;    // verilog tb for moore state machine.   
   reg reset_n_wire;
   reg clk_wire;
   reg x_wire;
   wire z_wire;
   integer i;

   moore uut  (                        
               .RESET_N(reset_n_wire),     // instantiation of state machine
               .X(x_wire),                 // with explicit port mapping.
               .CLK(clk_wire),        
               .Z(z_wire)
              );
   
   initial
      begin
         clk_wire = 0;
         forever #10 clk_wire = ~clk_wire;      // create clock.  
      end                                       // ~ is bitwise negation operator.

   initial  
      begin
         reset_n_wire = 0;
         x_wire = 0;
         # 30;
         reset_n_wire = 1;
         for (i=0; i<5; i=i+1)
         begin
            @(negedge clk_wire)  // use the negative edge in a testbench to change.
            x_wire = 1;          // input values to a synchronous design under test.
            @(negedge clk_wire)  
            x_wire = 0;
            @(negedge clk_wire)  
            x_wire = 0;
            @(negedge clk_wire)  
            x_wire = 1;     
         end
         $stop                    // system task to end simulation.
      end 

endmodule
      

