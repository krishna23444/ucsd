/**********************************************************************/
/*                    Px Generated File                               */
/*              Platform Express, Version 3.3.1 (BuildId: 200607071143)*/
/*              System Level Engineering Division                     */
/*              Mentor Graphics Corporation                           */
/*                                                                    */
/* Generated on: February 18, 2007 7:18:39 PM PST                     */
/* Generated by: saumya                                               */
/* Generated bus decoder for bus AHB_1 using decoder template AHB_priorityArbiter_vlog.xml*/
/**********************************************************************/


`timescale 1ns/1ns
module PxDecoder_AHB_1_1407810334_module(
	a946_1_CLK,
	AhbMemory_1_HCLK,
	AhbMemory_2_HCLK,
	a946_1_HRESETn,
	AhbMemory_1_HRESETn,
	AhbMemory_2_HRESETn,
	a946_1_HADDR,
	AhbMemory_1_HADDR,
	AhbMemory_2_HADDR,
	a946_1_HTRANS,
	AhbMemory_1_HTRANS,
	AhbMemory_2_HTRANS,
	a946_1_HWRITE,
	AhbMemory_1_HWRITE,
	AhbMemory_2_HWRITE,
	a946_1_HSIZE,
	AhbMemory_1_HSIZE,
	AhbMemory_2_HSIZE,
	a946_1_HBURST,
	a946_1_HPROT,
	a946_1_HWDATA,
	AhbMemory_1_HWDATA,
	AhbMemory_2_HWDATA,
	AhbMemory_1_HSEL,
	AhbMemory_2_HSEL,
	a946_1_HLOCK,
	a946_1_HRDATA,
	AhbMemory_1_HRDATA,
	AhbMemory_2_HRDATA,
	AhbMemory_1_HREADYin,
	AhbMemory_2_HREADYin,
	a946_1_HREADY,
	AhbMemory_1_HREADYout,
	AhbMemory_2_HREADYout,
	a946_1_HRESP,
	AhbMemory_1_HRESP,
	AhbMemory_2_HRESP,
	a946_1_HBUSREQ,
	a946_1_HGRANT,
	AHB_1_ambaAHB_HCLK,
	AHB_1_ambaAHB_HRESETn 

);

// Port Declaration.
    output [0:0]                   a946_1_CLK;
    output [0:0]                   AhbMemory_1_HCLK;
    output [0:0]                   AhbMemory_2_HCLK;
    output [0:0]                   a946_1_HRESETn;
    output [0:0]                   AhbMemory_1_HRESETn;
    output [0:0]                   AhbMemory_2_HRESETn;
    input  [31:0]                  a946_1_HADDR;
    output [31:0]                  AhbMemory_1_HADDR;
    output [31:0]                  AhbMemory_2_HADDR;
    input  [1:0]                   a946_1_HTRANS;
    output [1:0]                   AhbMemory_1_HTRANS;
    output [1:0]                   AhbMemory_2_HTRANS;
    input  [0:0]                   a946_1_HWRITE;
    output [0:0]                   AhbMemory_1_HWRITE;
    output [0:0]                   AhbMemory_2_HWRITE;
    input  [2:0]                   a946_1_HSIZE;
    output [2:0]                   AhbMemory_1_HSIZE;
    output [2:0]                   AhbMemory_2_HSIZE;
    input  [2:0]                   a946_1_HBURST;
    input  [3:0]                   a946_1_HPROT;
    input  [31:0]                  a946_1_HWDATA;
    output [31:0]                  AhbMemory_1_HWDATA;
    output [31:0]                  AhbMemory_2_HWDATA;
    output [0:0]                   AhbMemory_1_HSEL;
    output [0:0]                   AhbMemory_2_HSEL;
    input  [0:0]                   a946_1_HLOCK;
    output [31:0]                  a946_1_HRDATA;
    input  [31:0]                  AhbMemory_1_HRDATA;
    input  [31:0]                  AhbMemory_2_HRDATA;
    output [0:0]                   AhbMemory_1_HREADYin;
    output [0:0]                   AhbMemory_2_HREADYin;
    output [0:0]                   a946_1_HREADY;
    input  [0:0]                   AhbMemory_1_HREADYout;
    input  [0:0]                   AhbMemory_2_HREADYout;
    output [1:0]                   a946_1_HRESP;
    input  [1:0]                   AhbMemory_1_HRESP;
    input  [1:0]                   AhbMemory_2_HRESP;
    input  [0:0]                   a946_1_HBUSREQ;
    output [0:0]                   a946_1_HGRANT;
    input  [0:0]                   AHB_1_ambaAHB_HCLK;
    input  [0:0]                   AHB_1_ambaAHB_HRESETn;


    wire id_hclk;
    wire id_hresetn;


    // Here are some constants specifying the expected size of some signals
    // Some interfaces only partially implement these signals so the resize() function
    // is used to normalise the width to these values.

    `define SIZE_HSIZE  3
    `define SIZE_HTRANS 2
    `define SIZE_HRESP  2
    `define SIZE_HBURST 3
    `define SIZE_HPROT  4

    // Here are some constants for mode signals on the AMBA bus.
    // HTRANS transfer type signal encoding
    `define  AMBA_HTRANS_IDLE    2'b00
    `define  AMBA_HTRANS_BUSY    2'b01
    `define  AMBA_HTRANS_NONSEQ  2'b10
    `define  AMBA_HTRANS_SEQ     2'b11

    // HBURST Types
    `define  AMBA_HBURST_SINGLE 3'b000
    `define  AMBA_HBURST_INCR   3'b001
    `define  AMBA_HBURST_WRAP4  3'b010
    `define  AMBA_HBURST_INCR4  3'b011
    `define  AMBA_HBURST_WRAP8  3'b100
    `define  AMBA_HBURST_INCR8  3'b101
    `define  AMBA_HBURST_WRAP16 3'b110
    `define  AMBA_HBURST_INCR16 3'b111

    // HRESP transfer response
    `define  AMBA_HRESP_OK       2'b00
    `define  AMBA_HRESP_ERR      2'b01
    `define  AMBA_HRESP_RETRY    2'b10
    `define  AMBA_HRESP_SPLIT    2'b11

    // an array of '0's for setting default values
    wire [31:0] zero = 32'h0;

    // HSELx_Delayed contains a local copy of the HSELx signal for each slave on the bus,
    // even if the slave does not implement HSELx. The HSELx bits are used to select
    // all the HRDATA, HRESP, and HREADY multiplexer input signals that flow back from 
    // the slaves to the master
    // 
    // A set of 'HSELx_Current' signals are used to decode the current address. The current address
    // is used by data on the next cycle, so HSELx_Delayed is the 'delayed' version of 
    //
    // HSELx_Current is the HSELx signals decoded from the currently transmitted address. 
    // 
    // HSELx_CurrentInternal is the HSELx signal that activates when any of addresses reserved by
    // the master are accessed
    //
    // HSELx_CurrentDefaultSlave is the HSELx signal that activates when no other HSELx is 
    // activated. This switches in all defaultSlave activities (a wrong address has been accessed). 
 
    reg  [2-1:0] HSELx_Delayed;
    reg  HSELx_DelayedDefaultSlave;

    wire [2-1:0] HSELx_Current; 
    wire HSELx_CurrentInternal;
    wire HSELx_CurrentDefaultSlave;

    // The signals used for the default slave
    wire HREADY_DefaultSlave;
    wire defaultSlave_CurrentTransfer;                // detect TRANS settings for OK and ERR RESP responses
    reg  defaultSlave_DelayedTransfer;
    wire [1:0] HRESP_DefaultSlave = `AMBA_HRESP_OK ;  // Generated response is always OK


    // Maximum nunber of masters is 16
    reg  [15:0] HMASTER_CONTROL_select;
    reg  [15:0] HMASTER_DATA_select;
    reg  [3:0]  id_hmaster;
    reg  [3:0]  hmaster_delay;
    reg  [15:0] id_hgrant;
    wire [15:0] id_hbusreq;
    wire [15:0] id_hlock;

    reg  [5:1] nvalid; 
    reg  [3:0] nmst [5:1];
    wire [3:0] defaultMaster = 4'b0000;
    reg  [3:0] nhMaster;


  wire [15:0] id_hsplit;

  reg [1:0] id_hresp;

   reg [31:0] id_hrdata;

    reg id_hwrite;

   reg [31:0] id_haddr;

   reg [1:0] id_htrans;

   reg [2:0] id_hburst;

   reg [2:0] id_hsize;

   reg [4:0] id_hprot;

   reg [31:0] id_hwdata;

  reg id_hreadyout; 

    // clock for all synchronous logic inside this decoder.
    assign id_hclk = AHB_1_ambaAHB_HCLK;

   // HCLK assignment for master -----------------------------------------------
   assign a946_1_CLK = id_hclk;

   // HCLK assignment for slave ------------------------------------------------
   assign AhbMemory_1_HCLK = id_hclk;

   // HCLK assignment for slave ------------------------------------------------
   assign AhbMemory_2_HCLK = id_hclk;

    // reset for all synchronous logic inside this decoder.
    assign id_hresetn = AHB_1_ambaAHB_HRESETn;

   // HRESETn assignment for master --------------------------------------------
   assign a946_1_HRESETn = id_hresetn;

   // HRESETn assignment for slave ---------------------------------------------
   assign AhbMemory_1_HRESETn = id_hresetn;

   // HRESETn assignment for slave ---------------------------------------------
   assign AhbMemory_2_HRESETn = id_hresetn;

  // Arbiter ---------------------------------------------------------------------------------------------
  // Here is the guts of the arbiter. Thanks to Jiri Gaisler and the Leon project for the original code
  // from which this was developed.
  // 
  // Implements a priority arbitration scheme
  //------------------------------------------------------------------------------------------------------

  
  assign id_hbusreq[0] = a946_1_HBUSREQ[0];
  assign id_hlock[0]   = a946_1_HLOCK[0];
  
  assign id_hbusreq[1] = zero[0];
  assign id_hlock[1]   = zero[0];
  
  assign id_hbusreq[2] = zero[0];
  assign id_hlock[2]   = zero[0];
  
  assign id_hbusreq[3] = zero[0];
  assign id_hlock[3]   = zero[0];
  
  assign id_hbusreq[4] = zero[0];
  assign id_hlock[4]   = zero[0];
  
  assign id_hbusreq[5] = zero[0];
  assign id_hlock[5]   = zero[0];
  
  assign id_hbusreq[6] = zero[0];
  assign id_hlock[6]   = zero[0];
  
  assign id_hbusreq[7] = zero[0];
  assign id_hlock[7]   = zero[0];
  
  assign id_hbusreq[8] = zero[0];
  assign id_hlock[8]   = zero[0];
  
  assign id_hbusreq[9] = zero[0];
  assign id_hlock[9]   = zero[0];
  
  assign id_hbusreq[10] = zero[0];
  assign id_hlock[10]   = zero[0];
  
  assign id_hbusreq[11] = zero[0];
  assign id_hlock[11]   = zero[0];
  
  assign id_hbusreq[12] = zero[0];
  assign id_hlock[12]   = zero[0];
  
  assign id_hbusreq[13] = zero[0];
  assign id_hlock[13]   = zero[0];
  
  assign id_hbusreq[14] = zero[0];
  assign id_hlock[14]   = zero[0];
  
  assign id_hbusreq[15] = zero[0];
  assign id_hlock[15]   = zero[0];
  
 
  // find next master
  // re-arbitrate on non-sequential accesses or when BUSY is seen
  // with the following priority:
  //      1. busreq and (htrans /= idle) and (htrans /= busy) and (split = 0)
  //      2. busreq and (htrans /= idle) and (split = 0)
  //      3. busreq and (split = 0)
  //      4. default master 

  integer i;

  always @(posedge id_hclk)  begin
    nvalid[4:1] = 4'b0000;
    nvalid[5] = 1'b1;
    nmst[5] = defaultMaster;
    for (i=0; i < 16; i=i+1 )
      if (!id_hsplit[i]) begin
        if (id_hbusreq[i]) begin
          if (id_htrans != `AMBA_HTRANS_IDLE) begin
            if (id_htrans != `AMBA_HTRANS_BUSY) begin
              nmst[1] = i;
              nvalid[1] = 1'b1;
            end
            nmst[2] = i;
            nvalid[2] = 1'b1;
          end
          nmst[3] = i;
          nvalid[3] = 1'b1;
        end
        if (!((nmst[4] == defaultMaster) & nvalid[4])) begin
          nmst[4] = i;
          nvalid[4] = 1'b1;
        end
      end
  end

  always @(nvalid or nmst[1] or nmst[2] or nmst[3] or nmst[4] or nmst[5])   
    begin : LOOP
      for (i=1; i <= 5; i=i+1)
        if (nvalid[i]) begin
          nhMaster = nmst[i];
          disable LOOP;
        end
    end


  // HMASTER Generation ----------------------------------------------------------------------------------
  // The HMASTER index is output on id_hmaster. The HMASTER_select signal is a decoded version of the
  // the id_hmaster signal. id_hmaster is passed to every split-capable slave.
  //------------------------------------------------------------------------------------------------------

  

  // generate an individual 'selects' of the hmaster number for easier decoding of mastermux functionality
  always @(posedge id_hclk or negedge id_hresetn)
    if (!id_hresetn)
      id_hmaster <= 'h0;
    else
      id_hmaster <= nhMaster;

    

  
  // HMASTER_CONTROL_select
  always @(posedge id_hclk or negedge id_hresetn)
    if (!id_hresetn)
      HMASTER_CONTROL_select <= 0;
    else if (id_hreadyout)
      case (id_hmaster)
        4'b0000: HMASTER_CONTROL_select <= 16'b0000000000000001;
        4'b0001: HMASTER_CONTROL_select <= 16'b0000000000000010;
        4'b0010: HMASTER_CONTROL_select <= 16'b0000000000000100;
        4'b0011: HMASTER_CONTROL_select <= 16'b0000000000001000;
        4'b0100: HMASTER_CONTROL_select <= 16'b0000000000010000;
        4'b0101: HMASTER_CONTROL_select <= 16'b0000000000100000;
        4'b0110: HMASTER_CONTROL_select <= 16'b0000000001000000;
        4'b0111: HMASTER_CONTROL_select <= 16'b0000000010000000;
        4'b1000: HMASTER_CONTROL_select <= 16'b0000000100000000;
        4'b1001: HMASTER_CONTROL_select <= 16'b0000001000000000;
        4'b1010: HMASTER_CONTROL_select <= 16'b0000010000000000;
        4'b1011: HMASTER_CONTROL_select <= 16'b0000100000000000;
        4'b1100: HMASTER_CONTROL_select <= 16'b0001000000000000;
        4'b1101: HMASTER_CONTROL_select <= 16'b0010000000000000;
        4'b1110: HMASTER_CONTROL_select <= 16'b0100000000000000;
        4'b1111: HMASTER_CONTROL_select <= 16'b1000000000000000;
        default: HMASTER_CONTROL_select <= 16'bxxxxxxxxxxxxxxxx;
      endcase 

  // HMASTER_DATA_select
  always @(posedge id_hclk or negedge id_hresetn)
    if (!id_hresetn)
        HMASTER_DATA_select    <= 0;
    else if (id_hreadyout)
        HMASTER_DATA_select <= HMASTER_CONTROL_select;


  // HGRANT
  always @(posedge id_hclk or negedge id_hresetn)
    if (!id_hresetn)
      id_hgrant <= 'h0;
    else
      case (nhMaster)
        4'b0000 : id_hgrant <= 16'b0000000000000001;
        4'b0001 : id_hgrant <= 16'b0000000000000010;
        4'b0010 : id_hgrant <= 16'b0000000000000100;
        4'b0011 : id_hgrant <= 16'b0000000000001000;
        4'b0100 : id_hgrant <= 16'b0000000000010000;
        4'b0101 : id_hgrant <= 16'b0000000000100000;
        4'b0110 : id_hgrant <= 16'b0000000001000000;
        4'b0111 : id_hgrant <= 16'b0000000010000000;
        4'b1000 : id_hgrant <= 16'b0000000100000000;
        4'b1001 : id_hgrant <= 16'b0000001000000000;
        4'b1010 : id_hgrant <= 16'b0000010000000000;
        4'b1011 : id_hgrant <= 16'b0000100000000000;
        4'b1100 : id_hgrant <= 16'b0001000000000000;
        4'b1101 : id_hgrant <= 16'b0010000000000000;
        4'b1110 : id_hgrant <= 16'b0100000000000000;
        4'b1111 : id_hgrant <= 16'b1000000000000000;
      endcase  

  
    assign a946_1_HGRANT = id_hgrant[0];
    


  // HSELx Generation ------------------------------------------------------------------------------------
  // Each bit is set to '0' when not selected and during a reset,
  // and set to '1' when the address on HADDR falls within the specified address component range.
  //------------------------------------------------------------------------------------------------------
  
    assign HSELx_Current[0] = ((id_hresetn) && (
   // (id_haddr[31:0] >= 0x0) && (id_haddr[31:0] <= (0x0 + 0x3fffff))
   (
      (id_haddr[31:0] >= 32'h00000000) && 
      (id_haddr[31:0] <= (32'h00000000 + 32'h003fffff))
   )
)) ? 1'b1 : 1'b0;
  
    assign HSELx_Current[1] = ((id_hresetn) && (
   // (id_haddr[31:0] >= 0x1000000) && (id_haddr[31:0] <= (0x1000000 + 0x1fffff))
   (
      (id_haddr[31:0] >= 32'h01000000) && 
      (id_haddr[31:0] <= (32'h01000000 + 32'h001fffff))
   )
)) ? 1'b1 : 1'b0;
  
  
   // HSELx Signal Propogation To Slave ------------------------------------------------------------------
   // The HSELx pins for each slave is driven from here, if the slave requires an HSELx signal
   //-----------------------------------------------------------------------------------------------------
   
   assign AhbMemory_1_HSEL = HSELx_Current[0]; 
   
   assign AhbMemory_2_HSEL = HSELx_Current[1]; 
   

   
   // HSELx Internal Generation -------------------------------------------------------------------------
   // Becomes true when an address range implemented within the master is selected
   //----------------------------------------------------------------------------------------------------
   assign HSELx_CurrentInternal = (
   // (id_haddr[31:0] >= 0x0) && (id_haddr[31:0] <= (0x0 + 0x3fffff))
   (
      (id_haddr[31:0] >= 32'h00000000) && 
      (id_haddr[31:0] <= (32'h00000000 + 32'h003fffff))
   )

 || 

   // (id_haddr[31:0] >= 0x1000000) && (id_haddr[31:0] <= (0x1000000 + 0x1fffff))
   (
      (id_haddr[31:0] >= 32'h01000000) && 
      (id_haddr[31:0] <= (32'h01000000 + 32'h001fffff))
   )
) ? 1'b1 : 1'b0;

   // Current -> Delayed Address State Machine -----------------------------------------------------------
   // This state machine controls the propogation of signal values from the 'current' cycle (in which the
   // address is propogated) to the 'delayed' cycle (where the data is propogated). All data processing 
   // is calculated using the 'delayed' version of the signals. Current becomes Delayed on the rising edge
   // of HCLK.
   //-----------------------------------------------------------------------------------------------------

   always @(negedge id_hresetn or posedge id_hclk)
     begin
       if (!id_hresetn)
         begin
           // Reset all values to default during reset
           HSELx_Delayed <= 'h0;
           HSELx_DelayedDefaultSlave <= 1'b0;
           defaultSlave_DelayedTransfer <= 1'b0; 
         end
       else if (id_hreadyout)
         begin
           // its a new bus cycle, so update all the delayed signals with the current signals     
           HSELx_Delayed <= HSELx_Current;
           HSELx_DelayedDefaultSlave <= HSELx_CurrentDefaultSlave;
           defaultSlave_DelayedTransfer <= defaultSlave_CurrentTransfer;
         end
     end


   // Default Slave ---------------------------------------------------------------------------------------
   //
   // The Default Slave is an auto-generated component which activates to terminate a bus cycle when an 
   // invalid address is accessed. The default slave provides HREADY and HRESP signals. HRDATA is set to
   // 0 in the HRDATA multiplexer (default value when no slaves are selected).
   //------------------------------------------------------------------------------------------------------
   
   // HSELx_CurrentDefaultSlave is true when none of HSELx_Current or HSELx_CurrentInternal (the address
   // ranges occupied inside the master) is active.
   assign HSELx_CurrentDefaultSlave = ~ (( | HSELx_Current) | HSELx_CurrentInternal);

   // Print out a warning message in case the designer does not realise that the default slave has
   // been selected. We can do this directly from the address.
   always @(HSELx_CurrentDefaultSlave)
     if (HSELx_CurrentDefaultSlave != 1'b0)
         $display ("Default Slave has been activated : possible incorrect address decoding");

   // Work out responses :- default slave should set HRESP to OK for IDLE or BUSY cycles, and ERR for SEQ or NONSEQ
   // cycles.
   assign defaultSlave_CurrentTransfer = ((HSELx_CurrentDefaultSlave == 1'b1) && (id_htrans == `AMBA_HTRANS_NONSEQ || id_htrans == `AMBA_HTRANS_SEQ)) ? 1'b1 : 1'b0;


   // HRESP returns AMBA_HRESP_ERR when a Sequential or non-Sequential transfer is detected
   assign HRESP_DefaultSlave = (defaultSlave_DelayedTransfer == 1'b1) ? `AMBA_HRESP_ERR : `AMBA_HRESP_OK;

   // The default slave always provides a single cycle response, so no requirement to extend the cycle.
   assign HREADY_DefaultSlave = 1'b1;


   // HSPLIT Combiner --------------------------------------------------------------------------------------
   //
   // This block or's together all the HSPLIT signals from the SPLIT capable slaves. The resultant id_hsplit
   // signal is an input to the arbiter.
   //
   //-------------------------------------------------------------------------------------------------------
   
   assign id_hsplit = (
            'h0
         | 
            'h0
        );


  assign a946_1_HRESP = id_hresp;

   // HRESP Multiplexer  ----------------------------------------------------------------------------------
   //
   // The multiplexer returns the HRESP signal from the selected slave (or "00" == AMBA_HRESP_OK if the 
   // slave does not implement HRESP), or the HRESP from the default slave if no slave deviced was selected, 
   // or AMBA_HRESP_OK if everything else fails.
   //------------------------------------------------------------------------------------------------------

   always @(HSELx_Delayed 
            or HSELx_DelayedDefaultSlave 
            or AhbMemory_1_HRESP
        
            or AhbMemory_2_HRESP
        
            or HRESP_DefaultSlave
            )
       if (HSELx_Delayed[0])
          id_hresp <= AhbMemory_1_HRESP;
       else if (HSELx_Delayed[1])
          id_hresp <= AhbMemory_2_HRESP;
       else if (HSELx_DelayedDefaultSlave)
          id_hresp <= HRESP_DefaultSlave;
       else
          id_hresp <= `AMBA_HRESP_OK;


  
   assign a946_1_HRDATA = id_hrdata;
  

   // HRDATA Multiplexer  ---------------------------------------------------------------------------------
   // 
   // The multiplexer returns the HRDATA signal from the selected slave (filled with '0's if the slave and
   // master HRDATA busses are different widths), or 0's if a read is attempted from an unselected device
   // (equivalent of reading the defaultSlave).
   //------------------------------------------------------------------------------------------------------

   always @(HSELx_Delayed 
            or AhbMemory_1_HRDATA
        
            or AhbMemory_2_HRDATA
        
            )
      if (HSELx_Delayed[0] == 1'b1)
        id_hrdata <= AhbMemory_1_HRDATA;
      else if (HSELx_Delayed[1] == 1'b1)
        id_hrdata <= AhbMemory_2_HRDATA;
      else
        id_hrdata <= 'h0;


   // HWRITE Signal Propogation To Slave 0---------------------------------------------
   assign AhbMemory_1_HWRITE = id_hwrite;

   // HWRITE Signal Propogation To Slave 1---------------------------------------------
   assign AhbMemory_2_HWRITE = id_hwrite;

   // HWRITE Mux from Master
   always @(HMASTER_CONTROL_select 
            or a946_1_HWRITE
        
            )  
      if (HMASTER_CONTROL_select[0])
         id_hwrite <= a946_1_HWRITE;
       else if (HMASTER_CONTROL_select[1])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[2])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[3])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[4])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[5])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[6])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[7])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[8])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[9])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[10])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[11])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[12])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[13])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[14])
         id_hwrite <= 1'b0;
       else if (HMASTER_CONTROL_select[15])
         id_hwrite <= 1'b0;
      else
         id_hwrite <= 1'b0;

   // HADDR Signal Propogation To Slave 0----------------------------------------------
   assign AhbMemory_1_HADDR = id_haddr[31:0];


   // HADDR Signal Propogation To Slave 1----------------------------------------------
   assign AhbMemory_2_HADDR = id_haddr[31:0];


   // HADDR Mux from Master - address is propogated from the master that has been granted the bus
   always @(HMASTER_CONTROL_select 
            or a946_1_HADDR
        
            )  
      if (HMASTER_CONTROL_select[0])
         id_haddr <= a946_1_HADDR;
       else if (HMASTER_CONTROL_select[1])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[2])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[3])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[4])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[5])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[6])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[7])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[8])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[9])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[10])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[11])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[12])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[13])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[14])
         id_haddr <= 'h0;
       else if (HMASTER_CONTROL_select[15])
         id_haddr <= 'h0;
      else
         id_haddr <= 'h0;

   // HTRANS Signal Propogation To Slave 0---------------------------------------------

   assign AhbMemory_1_HTRANS = id_htrans;

   // HTRANS Signal Propogation To Slave 1---------------------------------------------

   assign AhbMemory_2_HTRANS = id_htrans;

   // HTRANS Mux from Master
   always @(HMASTER_CONTROL_select 
            or a946_1_HTRANS
        
            )  
      if (HMASTER_CONTROL_select[0])
         id_htrans <= a946_1_HTRANS;
       else if (HMASTER_CONTROL_select[1])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[2])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[3])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[4])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[5])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[6])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[7])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[8])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[9])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[10])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[11])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[12])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[13])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[14])
         id_htrans <= `AMBA_HTRANS_IDLE;
       else if (HMASTER_CONTROL_select[15])
         id_htrans <= `AMBA_HTRANS_IDLE;
      else
         id_htrans <= `AMBA_HTRANS_IDLE;

   // HBURST Mux from Master
   always @(HMASTER_CONTROL_select 
            or a946_1_HBURST
        
            )  
      if (HMASTER_CONTROL_select[0])
         id_hburst <= a946_1_HBURST;
       else if (HMASTER_CONTROL_select[1])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[2])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[3])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[4])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[5])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[6])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[7])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[8])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[9])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[10])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[11])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[12])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[13])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[14])
         id_hburst <= 'h0;
       else if (HMASTER_CONTROL_select[15])
         id_hburst <= 'h0;
      else
         id_hburst <= 'h0;

   // HSIZE Signal Propogation To Slave 0-----------------------------------------------------------------
   assign AhbMemory_1_HSIZE = id_hsize;

   // HSIZE Signal Propogation To Slave 1-----------------------------------------------------------------
   assign AhbMemory_2_HSIZE = id_hsize;

   // HSIZE Mux from Master
   always @(HMASTER_CONTROL_select 
            or a946_1_HSIZE
        
            )  
      if (HMASTER_CONTROL_select[0])
         id_hsize <= a946_1_HSIZE;
       else if (HMASTER_CONTROL_select[1])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[2])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[3])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[4])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[5])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[6])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[7])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[8])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[9])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[10])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[11])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[12])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[13])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[14])
         id_hsize <= 'h0;
       else if (HMASTER_CONTROL_select[15])
         id_hsize <= 'h0;
      else
         id_hsize <= 'h0;

   // HPROT Mux from Master
   always @(HMASTER_CONTROL_select 
            or a946_1_HPROT
        
            )  
      if (HMASTER_CONTROL_select[0])
         id_hprot <= a946_1_HPROT;
       else if (HMASTER_CONTROL_select[1])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[2])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[3])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[4])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[5])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[6])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[7])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[8])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[9])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[10])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[11])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[12])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[13])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[14])
         id_hprot <= 'h0;
       else if (HMASTER_CONTROL_select[15])
         id_hprot <= 'h0;
      else
         id_hprot <= 'h0;

   // HWDATA Signal Propogation To Slave 0-----------------------------------------------------------------
   assign AhbMemory_1_HWDATA = id_hwdata;

   // HWDATA Signal Propogation To Slave 1-----------------------------------------------------------------
   assign AhbMemory_2_HWDATA = id_hwdata;

   // HWDATA Mux from Master
   always @(HMASTER_DATA_select 
            or a946_1_HWDATA
        
            )  
      if (HMASTER_DATA_select[0])
         id_hwdata <= a946_1_HWDATA;
       else if (HMASTER_DATA_select[1])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[2])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[3])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[4])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[5])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[6])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[7])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[8])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[9])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[10])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[11])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[12])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[13])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[14])
         id_hwdata <= 'h0;
       else if (HMASTER_DATA_select[15])
         id_hwdata <= 'h0;
      else
         id_hwdata <= 'h0;

   // HREADYOUT Multiplexer  ------------------------------------------------------------------------------
   //
   // The multiplexer selects  the HREADYOut from the selected slave (this signal is required), or the 
   // HREADYout from the default slave if no slave deviced was selected, else '1' (for emergencies). 
   // HSELx_Delayed is used as the mux selector.
   // HREADYout is used to propogate the output of the multiplexer back to the master. The intermediate 
   // signal is used for
   //------------------------------------------------------------------------------------------------------

   always @(HSELx_Delayed 
            or HREADY_DefaultSlave 
            or AhbMemory_1_HREADYout
        
            or AhbMemory_2_HREADYout
        
            )
      if (HSELx_Delayed[0])
         id_hreadyout <= AhbMemory_1_HREADYout;
      else if (HSELx_Delayed[1])
         id_hreadyout <= AhbMemory_2_HREADYout;
      else if (HSELx_DelayedDefaultSlave)
         id_hreadyout <= HREADY_DefaultSlave;
      else
         id_hreadyout <= 1'b1;


   // We propogate the HREADY signal back to the active master only.
   assign a946_1_HREADY = id_hreadyout;

   
   // HREADY signal propogation for slave 0 -----------------------------------------
   // HREADY is the combination of the HREADYouts from all the slaves
   //-------------------------------------------------------------------------------------------------------
   assign AhbMemory_1_HREADYin = id_hreadyout;
   
   // HREADY signal propogation for slave 1 -----------------------------------------
   // HREADY is the combination of the HREADYouts from all the slaves
   //-------------------------------------------------------------------------------------------------------
   assign AhbMemory_2_HREADYin = id_hreadyout;
   

endmodule
