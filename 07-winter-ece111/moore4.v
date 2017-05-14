module moore4 (RESET_N, CLK, Z);
   input RESET_N, CLK;
   output Z;

   reg  Z;                // Z and STATE are assigned in always block => type reg. 
   reg  STATE;            

   parameter  s0=1'b0,      // 1'b0 denotes one bit with the value 0 in binary notation  
              s1=1'b1;   

   always @ (posedge CLK or negedge RESET_N)    
      if (! RESET_N)                          // asynchronous active low reset
         begin
            Z <= 1'b0;
            STATE <= s0;
         end
      else
         begin
	    case (STATE)        
	       s0: begin
		      Z <= 1'b0;           // registered output Z 
		      STATE <= s1;         // state register STATE 
		   end                     
	       s1: begin
		      Z <= 1'b1;
		      STATE <= s0;
		   end 
	    endcase 
	 end      

   endmodule
