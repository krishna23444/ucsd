// Verilog one-bit adder slice

module full_adder                     // module name 
       (a_one,      
        a_two,                        // module ports
        c_in, 
        s,
        c_out);                       

   input  a_one;            // by default a signal's number of bits is 1 
   input  a_two;           
   input  c_in;             // declaration of module ports
   output s;        
   output c_out;        

   wire s;                // declare wires            
   wire c_out;  

   assign  s = a_one ^ a_two ^ c_in;     // continuous signal assignment         
   assign  c_out = (a_one & a_two) | (a_one & c_in) | (a_two & c_in);  

endmodule
