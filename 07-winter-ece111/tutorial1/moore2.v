
// Verilog moore machine, state machine design style 2
// One unclocked process (always block) for combinational logic.
// One clocked process to update the state register. 
// One continuous assignment statement to assign unregistered output. 

module moore2 (RESET_N,CLK, Z);
   input RESET_N;
   input CLK;
   output Z;

   reg  CURRENT_STATE;     // Declared as type reg since assigned to in always block. 
   reg  NEXT_STATE;        // Synthesizes to  flip-flops only if the always block is 
                           // sensitive to an edge, i.e., posedge CLK.
                           // posedge is the keyword to signify rising edge. 
     
   parameter  s0=0,      // Assign states using a parameter statement. 
              s1=1;

   always @ (CURRENT_STATE)     // unclocked process (always assignment block)
      case (CURRENT_STATE)        
         s0:   NEXT_STATE=s1;   
         s1:   NEXT_STATE=s0;
      endcase 

 always @ (posedge CLK or negedge RESET_N)    // always block for updating state register
   begin
      if (! RESET_N)              
         CURRENT_STATE <= s0;                // asynchronous active low reset 
      else
         CURRENT_STATE <= NEXT_STATE;           
   end

   assign Z =  (CURRENT_STATE==s0) ? 1'b0: 1'b1;  // continuous signal assignment
                                               // Z is 0 if CURRENT_STATE is s0.
                                               // Z is 1 if CURRENT_STATE is s1. 

endmodule
