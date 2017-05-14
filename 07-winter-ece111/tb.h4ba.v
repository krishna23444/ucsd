module tb_h4ba;            // Verilog testbench to test hierarchical four-bit adder.
   reg [3:0] addend_one;   // Signals assigned to in the initial block are declared
   reg [3:0] addend_two;   // as type reg. 
   reg carry_in;           // If the number of bits is not specified, it is 1 bit.      
   reg flag;           
   wire [3:0] sum;
   wire carry_out;
   integer i;      
                                         
   h4ba uut (                            
             .addend_one(addend_one),        // Instantiation and port mapping.
             .addend_two(addend_two),        // .formal(actual)
             .carry_in(carry_in),
             .sum(sum),
             .carry_out(carry_out)
            );


    initial                    // initial block to apply input values to test h4ba module.
    begin                    
       carry_in = 0;             
       addend_one = 0;      
       addend_two = 0;     
       flag = 1'b0;                         // flag is 1 bit and that bit is set to 0.
       for (i=0; i<15; i=i+1)
       begin
	  # 20                                    
          $strobe("addend_one: %d   addend_two: %d   carry_in: %d",addend_one, addend_two, carry_in); 
          $strobe("sum: %d   carry_out %d", sum, carry_out); 
          if (((carry_out*16)+sum)==(addend_one+addend_two+carry_in))
             $strobe("This is correct.\n"); 
          else
             begin                                       // $strobe writes at end of time unit.
                $strobe("This is incorrect.\n");         // Use begin end pair for  
                flag = 1'b1;                             // multiple-statement branch. 
             end
	  addend_one = addend_one+1;      
	  addend_two = addend_two+1;     
          carry_in = ~carry_in;         
       end
       if (flag == 1'b0)
             $display("There were no errors.");           // $display writes instantaneously.  
          else
             $display("There was at least one error."); 
    $stop;                                             // System task to stop simulation.
    end   

endmodule 

