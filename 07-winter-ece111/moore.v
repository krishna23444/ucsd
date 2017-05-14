// Verilog moore machine, state machine design style 1
// One unclocked process (always assignment block) for combinational logic.
// One clocked process (always assignment block) for synchronous elements.

module moore (RESET_N, X, CLK, Z);
   input RESET_N;
   input X;
   input CLK;
   output Z;

   reg Z;                        // Variables written to in an always block
   reg  [1:0] CURRENT_STATE;     // are declared as type reg.  Will synthesize
   reg  [1:0] NEXT_STATE;        // to  flip-flops only if the always block is 
                                 // sensitive to an edge, i.e., posedge CLK.
                                // posedge is the keyword to signify rising edge. 

   parameter [1:0] s0=0,       // Define states using a parameter declaration. 
                   s1=1,
                   s2=2,
                   s3=3;

   always @ (CURRENT_STATE or X)     // unclocked process (always assignment block)
   begin   
      case (CURRENT_STATE)        
         s0: begin
                Z = 0;                         // The output Z is assigned.
                NEXT_STATE=(X==0) ? s0 : s2;      // The next state is assigned
             end                                  // s0 if X==0, s2 if X~=0. 
         s1: begin
                Z = 1;
                NEXT_STATE=(X==0) ? s0 : s2;
             end 
         s2: begin
                Z = 1;
                NEXT_STATE=(X==0) ? s2 : s3;
             end 
         s3: begin
                Z = 0;
                NEXT_STATE=(X==0) ? s3 : s1;
             end 
      endcase 
   end

   always @ (posedge CLK or negedge RESET_N)             // always block for updating state register
   begin
      if (! RESET_N)              
         CURRENT_STATE = s0;                // asynchronous active low reset
      else
         CURRENT_STATE = NEXT_STATE;           
   end

endmodule
