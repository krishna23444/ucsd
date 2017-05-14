module no_latch_example(A,B,Y,Z); 
   input  A,B;  
   output Y,Z; 
   reg    Y,Z; 

   always@(A or B)
   begin: p1 
     if (A)
        begin
           Y = B;
           Z = ~ B;
        end
     else 
        begin
           Y = B;
           Z = B;
        end  
     end 
						
endmodule      
