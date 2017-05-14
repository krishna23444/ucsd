module latch_example(A,B,Y,Z); 
   input   A,B; 
   output  Y,Z;
   reg     Y,Z;

   always@(A or B)
   begin: p0
     if (A) 
        begin
           Y = B;
           Z = ~ B;
        end
     else 
           Z = B;
     end  
						
endmodule
