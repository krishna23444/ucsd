
module tb_src;    // Verilog tb for sync up counter    

   reg clk_wire;
   reg reset_wire;
   wire [4:0] q_wire;         

   sync_reset_counter m1   (                      // instantiation of counter 
                           .clk(clk_wire),        // with explicit port mapping 
                           .reset(reset_wire),     
                           .q(q_wire)
                           );
   
   initial
      begin
         clk_wire = 0;
         forever #10 clk_wire = ~clk_wire;      // create clock
      end

   initial  
      begin
         reset_wire = 0;
         # 10;
         reset_wire = 1;
         # 30;
         reset_wire = 0;
      end 

endmodule
      

