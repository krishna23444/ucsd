// Verilog moore machine, state machine design style 5
// One unclocked process (always block) to assign output Z.
// One unclocked process (always block) to assign next state.
// One clocked process (always block w/ keyword posedge) for state register.  

module moore5 (RESET_N,CLK, Z);
   input RESET_N;
   input CLK;
   output Z;

   reg  CURRENT_STATE;     // Declared as type reg since assigned to in always block. 
   reg  NEXT_STATE;        // Synthesizes to  flip-flops only if the always block is 
   reg  Z;                 // sensitive to an edge, i.e., posedge CLK.
                           // posedge is the keyword to signify rising edge. 
     
   parameter  s0=0,        // Assign states using a parameter statement. 
              s1=1;

   always @ (CURRENT_STATE)    // unclocked process (always assignment block)
      case (CURRENT_STATE)        
         s0:  Z = 1'b0;        // unregistered output Z is 0 if CURRENT_STATE is s0. 
         s1:  Z = 1'b1;        // unregistered output Z is 1 if CURRENT_STATE is s1. 
      endcase 

   always @ (CURRENT_STATE)     // unclocked process (always assignment block)
      case (CURRENT_STATE)        
         s0:   NEXT_STATE=s1;   
         s1:   NEXT_STATE=s0;
      endcase 

   always @ (posedge CLK or negedge RESET_N)      // always block for updating state register
   begin
      if (! RESET_N)                              // asynchronous active low reset 
         CURRENT_STATE <= s0;             
      else
         CURRENT_STATE <= NEXT_STATE;           
   end
endmodule
