/*
 * A simple behavorial 32-bit AHB memory.
 */

`timescale 1ns/1ns

`define CVE

// HRESP codes
`define HRESP_OK    2'b00
`define HRESP_ERROR 2'b01
`define HRESP_RETRY 2'b10
`define HRESP_SPLIT 2'b11

// HTRANS codes
`define HTRANS_IDLE    2'b00
`define HTRANS_BUSY    2'b01
`define HTRANS_NONSEQ  2'b10
`define HTRANS_SEQ     2'b11

// HSIZE codes
`define HSIZE_BYTE     3'b000
`define HSIZE_HALF     3'b001
`define HSIZE_WORD     3'b010

module AhbMemory(
                HCLK,
                HRESETn,
                HSEL,
                HADDR, 
                HTRANS, 
                HWRITE, 
                HSIZE, 
                HREADYin, 
                HWDATA,

                HRDATA, 
                HREADYout, 
                HRESP);

   parameter    numAddrBits = 20;    // number of address bits (default 1M)
   parameter    waitStates  = 0;     // wait states for transfers

   input         HCLK;
   input         HRESETn;
   input         HSEL;
   input [31:0]  HADDR;
   input [1:0]   HTRANS;
   input         HWRITE;
   input [2:0]   HSIZE;
   input         HREADYin;
   input [31:0]  HWDATA;
   
   output [31:0] HRDATA;
   output        HREADYout;
   output [1:0]  HRESP;


   // Control signal state registers
   wire          selected = ~HRESETn ? 1'b0 : (HREADYin & HSEL);
   reg           hsel_reg;

   reg [31:0]    haddr_reg;
   reg [1:0]     htrans_reg;
   reg           hwrite_reg;
   reg [2:0]     hsize_reg;
   reg [31:0]    hrdata_reg;

   // memory transfer state signals
   wire [numAddrBits-3:0] memAddr_w = haddr_reg[numAddrBits-1:2];
   wire [numAddrBits-3:0] memAddr_r = HADDR[numAddrBits-1:2];
   reg [31:0]        memTmp_w;
   reg [31:0]        memTmp_r;



   // wait state variables
   reg [3:0]         waitCount;
   reg [3:0]         waitReload;

   // Output signal continuous assignments
   assign  HRDATA = (hsel_reg & ~hwrite_reg & HREADYout) ? hrdata_reg : 
                     32'h0000_0000;
   assign  HREADYout = hsel_reg ?  ( waitCount === 4'b0000 ) : 1'b0;
   assign  HRESP = hsel_reg ? 2'b00 : 2'b00;

`ifndef CVE
   reg [31:0]    mem[(1<<numAddrBits)-1:0];    // memory store
`endif
      

   initial begin
      waitReload  = waitStates;    // default wait cycle
`ifdef CVE
      $cve_RegisterMemory(numAddrBits-2,32);
`endif    
   end // initial begin
   

   // Latch control signals 
   always @(posedge HCLK)
      if(~HRESETn) begin
         haddr_reg <=  32'h0000_0000;
         htrans_reg <=  2'b00;
         hwrite_reg <=  1'b0;
         hsize_reg <=  2'b00;
         hsel_reg <=  1'b0;
      end
      else if (HREADYin) begin
         haddr_reg <=  HADDR;
         htrans_reg <=  HTRANS;
         hwrite_reg <=  HWRITE;
         hsize_reg <=  HSIZE;
         hsel_reg <=  selected;
      end


   // Wait state counter
   always @(posedge HCLK)
      if(~HRESETn)                
         waitCount <=  4'h0;
      else if (selected & HREADYin)  begin
         if (HTRANS[1])  
            waitCount <=  waitReload;
         else        
            waitCount <= 4'h0;
      end
           else if (hsel_reg&(waitCount != 4'b0000))   
              waitCount <=  waitCount-1;
                else
                   waitCount <=  4'h0;



   // Memory transfers
   always @(posedge HCLK)
      begin
         if(HRESETn & hwrite_reg & HREADYin & hsel_reg & htrans_reg[1] )
            begin // write operation

`ifdef CVE             
               $cve_ReadMemory(memAddr_w, memTmp_w);
`else              
               memTmp_w = mem[memAddr_w];
`endif             

               casex ({hsize_reg,haddr_reg[1:0]}) 
                 {`HSIZE_WORD, 2'bxx} : begin
                    memTmp_w = HWDATA;
                 end
                 {`HSIZE_HALF, 2'b0x} : begin
                    memTmp_w = { memTmp_w[31:16],HWDATA[15:0] };
                 end
                 {`HSIZE_HALF, 2'b1x} : begin
                    memTmp_w = { HWDATA[31:16],memTmp_w[15:0] };
                 end
                 {`HSIZE_BYTE, 2'b00} : begin
                    memTmp_w = { memTmp_w[31:8],HWDATA[7:0] };
                 end
                 {`HSIZE_BYTE, 2'b01} : begin
                    memTmp_w = { memTmp_w[31:16],HWDATA[15:8],memTmp_w[7:0] };
                 end
                 {`HSIZE_BYTE, 2'b10} : begin
                    memTmp_w = { memTmp_w[31:24],HWDATA[23:16],
                                 memTmp_w[15:0] };
                 end
                 {`HSIZE_BYTE, 2'b11} : begin
                    memTmp_w = { HWDATA[31:24],memTmp_w[23:0] };
                 end
                 default :   
                    $display ("ERROR: unsuported transfer HSIZE=%d: pxAhbMem.v ",HSIZE, $time);
               endcase // casex({hsize_reg,haddr_reg[1:0]})

`ifdef CVE
               $cve_WriteMemory(haddr_reg[numAddrBits-1:2], memTmp_w);
`else
               mem[memAddr_w] = memTmp_w;
`endif             

            end // write operation
         

         if(HRESETn & ~HWRITE & HREADYin & selected & ~HTRANS[1])
            begin
               hrdata_reg = 32'h0000_0000;
            end
         else if(HRESETn & ~HWRITE & HREADYin & selected & HTRANS[1])
            begin  // read operation
`ifdef CVE
               $cve_ReadMemory(memAddr_r, memTmp_r);
`else              
               memTmp_r = mem[memAddr_r];
`endif             

               casex ({HSIZE,HADDR[1:0]}) 
                 {`HSIZE_WORD, 2'bxx} : begin
                    hrdata_reg = memTmp_r;
                 end
                 {`HSIZE_HALF, 2'b0x} : begin
                    hrdata_reg = { memTmp_r[15:0],memTmp_r[15:0] };
                 end
                 {`HSIZE_HALF, 2'b1x} : begin
                    hrdata_reg = { memTmp_r[31:16],memTmp_r[31:16] };
                 end
                 {`HSIZE_BYTE, 2'b00} : begin
                    hrdata_reg = { memTmp_r[7:0],memTmp_r[7:0],
                                   memTmp_r[7:0],memTmp_r[7:0] };
                 end
                 {`HSIZE_BYTE, 2'b01} : begin
                    hrdata_reg = { memTmp_r[15:8],memTmp_r[15:8],
                                   memTmp_r[15:8],memTmp_r[15:8] };
                 end
                 {`HSIZE_BYTE, 2'b10} : begin
                    hrdata_reg = { memTmp_r[23:16],memTmp_r[23:16],
                                   memTmp_r[23:16],memTmp_r[23:16] };
                 end
                 {`HSIZE_BYTE, 2'b11} : begin
                    hrdata_reg = { memTmp_r[31:24],memTmp_r[31:24],
                                   memTmp_r[31:24],memTmp_r[31:24] };
                 end
                 default :   
                    $display ("ERROR: unsuported transfer HSIZE=%d: pxAhbMem.v ",HSIZE, $time);
               endcase
            end // read operation
      end // always @ (posedge HCLK)

endmodule

