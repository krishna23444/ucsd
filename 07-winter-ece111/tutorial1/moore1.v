module moore1 (RESET_N,CLK, Z);
   input RESET_N, CLK;
   output Z;

   reg  Z, CURRENT_STATE, NEXT_STATE;   // signals assigned to in an always block                

   parameter  s0=0, s1=1;     // parameter is used to declare constants     

   always @ (CURRENT_STATE) 
      case (CURRENT_STATE)  
         s0: begin
                Z = 0;   
                NEXT_STATE=s1;   
             end                     
         s1: begin
                Z = 1;
                NEXT_STATE=s0;
             end 
      endcase 

 always @ (posedge CLK or negedge RESET_N)      
   begin
      if (! RESET_N)               // ! is the logical negation operator.
         CURRENT_STATE <= s0;      // asynchronous active low reset.         
      else
         CURRENT_STATE <= NEXT_STATE;
   end

endmodule
