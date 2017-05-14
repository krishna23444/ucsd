// Verilog moore machine, state machine design style 3
// One unclocked process (always assignment block) for  
// for combinational logic to assign next state.
// One clocked process for state register.
// One clocked process to register output. 

module moore3 (RESET_N,CLK, Z);
   input RESET_N;
   input CLK;
   output Z;

   reg  CURRENT_STATE;     // Declared as type reg since assigned to in always block. 
   reg  NEXT_STATE;        // Synthesizes to  flip-flops only if the always block is 
   reg  Z;                 // sensitive to an edge, i.e., posedge CLK.
                           // posedge is the keyword to signify rising edge. 
     
   parameter  s0=0,      // Declare states using a parameters. 
              s1=1;

   always @ (CURRENT_STATE)     // unclocked process (always assignment block)
      case (CURRENT_STATE)        
          s0:    NEXT_STATE=s1;   
          s1:    NEXT_STATE=s0;
      endcase 

 always @ (posedge CLK or negedge RESET_N)     // always block for updating state register
   begin
      if (! RESET_N)              
         CURRENT_STATE <= s0;                // asynchronous active low reset
      else
         CURRENT_STATE <= NEXT_STATE;           
   end

   always @ (posedge CLK)     // clocked process (always assignment block)
      case (NEXT_STATE)       // to register output Z 
         s0:   Z <= 0 ;   
         s1:   Z <= 1;
      endcase 

endmodule
