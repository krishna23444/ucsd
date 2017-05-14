
// reg file of 4 regs
module regfile(clk,reset_n,nr_w,enable,addr,datain,dataout);
  input clk;
  input reset_n;
  input nr_w;     
  input enable;  
  input [1:0] addr;
  input [31:0] datain;     
  output [31:0] dataout;

  reg [31:0] dataout;
  reg [31:0] d1;
  reg [31:0] d2;
  reg [31:0] d3;
  reg [31:0] q0;
  reg [31:0] q1;
  reg [31:0] q2;
  reg [31:0] q3;
  reg [31:0] d0;


   reg32 r0 (
                .reset_n(reset_n),
                .clk(clk),
                .D(d0),
                .Q(q0)
               );
   reg32 r1 (
                .reset_n(reset_n),
                .clk(clk),
                .D(d1),
                .Q(q1)
               );
   reg32 r2 (
                .reset_n(reset_n),
                .clk(clk),
                .D(d2),
                .Q(q2)
               );
   reg32 r3 (
                .reset_n(reset_n),
                .clk(clk),
                .D(d3),
                .Q(q3)
               );


   // definition of the mux/decoder
   always @ (nr_w or enable or addr or datain or q0 or q1 or q2 or q3)
   begin
     if (enable == 1) 
     begin
	if (nr_w == 0)  // read
	   begin
	   case (addr) 
		 0 : dataout = q0;
		 1 : dataout = q1;
		 2 : dataout = q2;
		 3 : dataout = q3;
	   endcase
	   // registers keep their old value
	   d0 = q0;
	   d1 = q1;
	   d2 = q2;
	   d3 = q3;
	   end
	else    // write
	   begin
	   case (addr)   
	      0 : begin 
		  d0 = datain;
		  d1 = q1;
		  d2 = q2;
		  d3 = q3;
		  end
	      1 : begin
		  d0 = q0;
		  d1 = datain;
		  d2 = q2;
		  d3 = q3;
		  end
	      2 : begin     
		  d0 = q0;  
		  d1 = q1;
		  d2 = datain;
		  d3 = q3;
		  end
	      3:  begin     
		  d0 = q0;
		  d1 = q1;
		  d2 = q2;
		  d3 = datain;
		  end
	   endcase
	   case (addr)                
	      0 : dataout = q0;      
	      1 : dataout = q1;        // let dataout be based on addr
	      2 : dataout = q2;        // should really just be don't care
	      3 : dataout = q3;
	   endcase
	end   
        end
     else // enable is 0: no read or write
	begin 
	// registers keep their old value
	d0 = q0;
	d1 = q1;
	d2 = q2;
	d3 = q3;
	// let dataout be based on addr
	// should really just be don't care
	case (addr) 
	   0 : dataout = q0;
	   1 : dataout = q1;
	   2 : dataout = q2;
	   3 : dataout = q3;
	endcase
	end   
     end
 endmodule 
