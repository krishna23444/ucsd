// Platform Express Verilog Top level template
/**********************************************************************/
/*                    Px Generated File                               */
/*              Platform Express, Version 3.3.1 (BuildId: 200607071143)*/
/*              System Level Engineering Division                     */
/*              Mentor Graphics Corporation                           */
/*                                                                    */
/* Generated on: February 18, 2007 7:18:21 PM PST                     */
/* Generated by: saumya                                               */
/* Generated Top Level HDL                                            */
/**********************************************************************/



`timescale 1ns/1ps



module test3
(
	test3_ahb_1_HCLK,
	test3_ahb_1_HRESETn,
	test3_AHB_1_ambaAHB_HCLK,
	test3_AHB_1_ambaAHB_HRESETn 

)
;
////////////////////
// Port Declaration
////////////////////
    input                          test3_ahb_1_HCLK;
    input                          test3_ahb_1_HRESETn;
    input                          test3_AHB_1_ambaAHB_HCLK;
    input                          test3_AHB_1_ambaAHB_HRESETn;



////////////////////
// Signal Declaration
////////////////////

// 
// Signal Declaration for Component: a946_1
// 
    wire                 pxGen_a946_1_CLK;
    wire                 pxGen_a946_1_UnGatedCLK;
    wire                 pxGen_a946_1_HCLKEN;
    wire                 pxGen_a946_1_HGRANT;
    wire[31:0]           pxGen_a946_1_HRDATA;
    wire                 pxGen_a946_1_HREADY;
    wire                 pxGen_a946_1_HRESETn;
    wire[1:0]            pxGen_a946_1_HRESP;
    wire                 pxGen_a946_1_nFIQ;
    wire                 pxGen_a946_1_nIRQ;
    wire[31:0]           pxGen_a946_1_TAPID;
    wire                 pxGen_a946_1_VINITHI;
    wire                 pxGen_a946_1_INITRAM;
    wire[3:0]            pxGen_a946_1_DCacheSize;
    wire[3:0]            pxGen_a946_1_ICacheSize;
    wire[1:0]            pxGen_a946_1_CHSDE;
    wire[1:0]            pxGen_a946_1_CHSEX;
    wire[31:0]           pxGen_a946_1_CPDIN;
    wire                 pxGen_a946_1_DBGDEWPT;
    wire                 pxGen_a946_1_DBGEN;
    wire[1:0]            pxGen_a946_1_DBGEXT;
    wire                 pxGen_a946_1_DBGIEBKPT;
    wire                 pxGen_a946_1_DBGSDOUT;
    wire                 pxGen_a946_1_DBGTCKEN;
    wire                 pxGen_a946_1_DBGTDI;
    wire                 pxGen_a946_1_DBGTMS;
    wire                 pxGen_a946_1_DBGnTRST;
    wire                 pxGen_a946_1_SI;
    wire                 pxGen_a946_1_SCANEN;
    wire                 pxGen_a946_1_TESTMODE;
    wire                 pxGen_a946_1_SERIALEN;
    wire                 pxGen_a946_1_TESTEN;
    wire                 pxGen_a946_1_INnotEXTEST;
    wire                 pxGen_a946_1_ETMFIFOFULL;
    wire                 pxGen_a946_1_EDBGRQ;
    wire                 pxGen_a946_1_ETMEN;
    wire[3:0]            pxGen_a946_1_PhyDTCMSize;
    wire[31:0]           pxGen_a946_1_DTCMRData;
    wire[3:0]            pxGen_a946_1_PhyITCMSize;
    wire[31:0]           pxGen_a946_1_ITCMRData;
    wire                 pxGen_a946_1_GateTheCLK;
    wire[31:0]           pxGen_a946_1_HADDR;
    wire[2:0]            pxGen_a946_1_HBURST;
    wire                 pxGen_a946_1_HLOCK;
    wire[3:0]            pxGen_a946_1_HPROT;
    wire                 pxGen_a946_1_HBUSREQ;
    wire[2:0]            pxGen_a946_1_HSIZE;
    wire[1:0]            pxGen_a946_1_HTRANS;
    wire[31:0]           pxGen_a946_1_HWDATA;
    wire                 pxGen_a946_1_HWRITE;
    wire                 pxGen_a946_1_BIGENDOUT;
    wire                 pxGen_a946_1_COMMRX;
    wire                 pxGen_a946_1_COMMTX;
    wire                 pxGen_a946_1_nCPMREQ;
    wire                 pxGen_a946_1_nCPTRANS;
    wire                 pxGen_a946_1_CPCLKEN;
    wire[31:0]           pxGen_a946_1_CPDOUT;
    wire[31:0]           pxGen_a946_1_CPINSTR;
    wire                 pxGen_a946_1_CPLATECANCEL;
    wire                 pxGen_a946_1_CPPASS;
    wire                 pxGen_a946_1_CPTBIT;
    wire                 pxGen_a946_1_SO;
    wire                 pxGen_a946_1_DBGACK;
    wire                 pxGen_a946_1_DBGINSTREXEC;
    wire[3:0]            pxGen_a946_1_DBGIR;
    wire[1:0]            pxGen_a946_1_DBGRNG;
    wire                 pxGen_a946_1_DBGRQI;
    wire[4:0]            pxGen_a946_1_DBGSCREG;
    wire                 pxGen_a946_1_DBGSDIN;
    wire[3:0]            pxGen_a946_1_DBGTAPSM;
    wire                 pxGen_a946_1_DBGTDO;
    wire                 pxGen_a946_1_DBGnTDOEN;
    wire                 pxGen_a946_1_ETMBIGEND;
    wire[1:0]            pxGen_a946_1_ETMCHSD;
    wire[1:0]            pxGen_a946_1_ETMCHSE;
    wire                 pxGen_a946_1_ETMDABORT;
    wire[31:0]           pxGen_a946_1_ETMDA;
    wire                 pxGen_a946_1_ETMDBGACK;
    wire[1:0]            pxGen_a946_1_ETMDMAS;
    wire                 pxGen_a946_1_ETMDMORE;
    wire                 pxGen_a946_1_ETMDSEQ;
    wire                 pxGen_a946_1_ETMDnMREQ;
    wire                 pxGen_a946_1_ETMDnRW;
    wire                 pxGen_a946_1_ETMHIVECS;
    wire                 pxGen_a946_1_ETMIABORT;
    wire[30:0]           pxGen_a946_1_ETMIA;
    wire[6:0]            pxGen_a946_1_ETMID31To25;
    wire[4:0]            pxGen_a946_1_ETMID15To11;
    wire                 pxGen_a946_1_ETMINSTREXEC;
    wire                 pxGen_a946_1_ETMISEQ;
    wire                 pxGen_a946_1_ETMITBIT;
    wire                 pxGen_a946_1_ETMInMREQ;
    wire                 pxGen_a946_1_ETMLATECANCEL;
    wire                 pxGen_a946_1_ETMPASS;
    wire[31:0]           pxGen_a946_1_ETMPROCID;
    wire                 pxGen_a946_1_ETMPROCIDWR;
    wire                 pxGen_a946_1_ETMINSTRVALID;
    wire[31:0]           pxGen_a946_1_ETMRDATA;
    wire[1:0]            pxGen_a946_1_ETMRNGOUT;
    wire[31:0]           pxGen_a946_1_ETMWDATA;
    wire                 pxGen_a946_1_ETMnWAIT;
    wire[31:0]           pxGen_a946_1_DTCMWData;
    wire[3:0]            pxGen_a946_1_DTCMWEn;
    wire                 pxGen_a946_1_DTCMEn;
    wire[17:0]           pxGen_a946_1_DTCMAdrs;
    wire[31:0]           pxGen_a946_1_ITCMWData;
    wire[3:0]            pxGen_a946_1_ITCMWEn;
    wire                 pxGen_a946_1_ITCMEn;
    wire[17:0]           pxGen_a946_1_ITCMAdrs;


// 
// Signal Declaration for Component: AhbMemory_1
// 
    wire                 pxGen_AhbMemory_1_HCLK;
    wire                 pxGen_AhbMemory_1_HRESETn;
    wire[31:0]           pxGen_AhbMemory_1_HADDR;
    wire[1:0]            pxGen_AhbMemory_1_HTRANS;
    wire                 pxGen_AhbMemory_1_HWRITE;
    wire[2:0]            pxGen_AhbMemory_1_HSIZE;
    wire[31:0]           pxGen_AhbMemory_1_HWDATA;
    wire                 pxGen_AhbMemory_1_HSEL;
    wire                 pxGen_AhbMemory_1_HREADYin;
    wire[31:0]           pxGen_AhbMemory_1_HRDATA;
    wire                 pxGen_AhbMemory_1_HREADYout;
    wire[1:0]            pxGen_AhbMemory_1_HRESP;


// 
// Signal Declaration for Component: AhbMemory_2
// 
    wire                 pxGen_AhbMemory_2_HCLK;
    wire                 pxGen_AhbMemory_2_HRESETn;
    wire[31:0]           pxGen_AhbMemory_2_HADDR;
    wire[1:0]            pxGen_AhbMemory_2_HTRANS;
    wire                 pxGen_AhbMemory_2_HWRITE;
    wire[2:0]            pxGen_AhbMemory_2_HSIZE;
    wire[31:0]           pxGen_AhbMemory_2_HWDATA;
    wire                 pxGen_AhbMemory_2_HSEL;
    wire                 pxGen_AhbMemory_2_HREADYin;
    wire[31:0]           pxGen_AhbMemory_2_HRDATA;
    wire                 pxGen_AhbMemory_2_HREADYout;
    wire[1:0]            pxGen_AhbMemory_2_HRESP;


// 
// Signal Declaration for Decoder: PxDecoder_AHB_1_1407810334
// 
// Px generated required driver signals




////////////////////
// Component instantiation
////////////////////

// 
// Component: a946_1 Instantiation
// 

// 
//  Default value assignement for signals
// 
    assign pxGen_a946_1_UnGatedCLK = 1'b0;
    assign pxGen_a946_1_HCLKEN = 1'b1;
    assign pxGen_a946_1_nFIQ = 1'b1;
    assign pxGen_a946_1_nIRQ = 1'b1;
    assign pxGen_a946_1_TAPID = 32'h00000000;
    assign pxGen_a946_1_VINITHI = 1'b0;
    assign pxGen_a946_1_INITRAM = 1'b0;
    assign pxGen_a946_1_DCacheSize = 4'b0000;
    assign pxGen_a946_1_ICacheSize = 4'b0000;
    assign pxGen_a946_1_CHSDE = 2'b10;
    assign pxGen_a946_1_CHSEX = 2'b10;
    assign pxGen_a946_1_CPDIN = 32'h00000000;
    assign pxGen_a946_1_DBGDEWPT = 1'b0;
    assign pxGen_a946_1_DBGEN = 1'b0;
    assign pxGen_a946_1_DBGEXT = 2'b11;
    assign pxGen_a946_1_DBGIEBKPT = 1'b0;
    assign pxGen_a946_1_DBGSDOUT = 1'b0;
    assign pxGen_a946_1_DBGTCKEN = 1'b0;
    assign pxGen_a946_1_DBGTDI = 1'b0;
    assign pxGen_a946_1_DBGTMS = 1'b0;
    assign pxGen_a946_1_DBGnTRST = 1'b1;
    assign pxGen_a946_1_SI = 1'b0;
    assign pxGen_a946_1_SCANEN = 1'b0;
    assign pxGen_a946_1_TESTMODE = 1'b0;
    assign pxGen_a946_1_SERIALEN = 1'b0;
    assign pxGen_a946_1_TESTEN = 1'b0;
    assign pxGen_a946_1_INnotEXTEST = 1'b0;
    assign pxGen_a946_1_ETMFIFOFULL = 1'b0;
    assign pxGen_a946_1_EDBGRQ = 1'b0;
    assign pxGen_a946_1_ETMEN = 1'b0;
    assign pxGen_a946_1_PhyDTCMSize = 4'b0000;
    assign pxGen_a946_1_DTCMRData = 32'h0000ffff;
    assign pxGen_a946_1_PhyITCMSize = 4'b0000;
    assign pxGen_a946_1_ITCMRData = 32'h0000ffff;
a946  a946_1(
    .CLK                      (pxGen_a946_1_CLK),
    .UnGatedCLK               (pxGen_a946_1_UnGatedCLK),
    .HCLKEN                   (pxGen_a946_1_HCLKEN),
    .HGRANT                   (pxGen_a946_1_HGRANT),
    .HRDATA                   (pxGen_a946_1_HRDATA),
    .HREADY                   (pxGen_a946_1_HREADY),
    .HRESETn                  (pxGen_a946_1_HRESETn),
    .HRESP                    (pxGen_a946_1_HRESP),
    .nFIQ                     (pxGen_a946_1_nFIQ),
    .nIRQ                     (pxGen_a946_1_nIRQ),
    .TAPID                    (pxGen_a946_1_TAPID),
    .VINITHI                  (pxGen_a946_1_VINITHI),
    .INITRAM                  (pxGen_a946_1_INITRAM),
    .DCacheSize               (pxGen_a946_1_DCacheSize),
    .ICacheSize               (pxGen_a946_1_ICacheSize),
    .CHSDE                    (pxGen_a946_1_CHSDE),
    .CHSEX                    (pxGen_a946_1_CHSEX),
    .CPDIN                    (pxGen_a946_1_CPDIN),
    .DBGDEWPT                 (pxGen_a946_1_DBGDEWPT),
    .DBGEN                    (pxGen_a946_1_DBGEN),
    .DBGEXT                   (pxGen_a946_1_DBGEXT),
    .DBGIEBKPT                (pxGen_a946_1_DBGIEBKPT),
    .DBGSDOUT                 (pxGen_a946_1_DBGSDOUT),
    .DBGTCKEN                 (pxGen_a946_1_DBGTCKEN),
    .DBGTDI                   (pxGen_a946_1_DBGTDI),
    .DBGTMS                   (pxGen_a946_1_DBGTMS),
    .DBGnTRST                 (pxGen_a946_1_DBGnTRST),
    .SI                       (pxGen_a946_1_SI),
    .SCANEN                   (pxGen_a946_1_SCANEN),
    .TESTMODE                 (pxGen_a946_1_TESTMODE),
    .SERIALEN                 (pxGen_a946_1_SERIALEN),
    .TESTEN                   (pxGen_a946_1_TESTEN),
    .INnotEXTEST              (pxGen_a946_1_INnotEXTEST),
    .ETMFIFOFULL              (pxGen_a946_1_ETMFIFOFULL),
    .EDBGRQ                   (pxGen_a946_1_EDBGRQ),
    .ETMEN                    (pxGen_a946_1_ETMEN),
    .PhyDTCMSize              (pxGen_a946_1_PhyDTCMSize),
    .DTCMRData                (pxGen_a946_1_DTCMRData),
    .PhyITCMSize              (pxGen_a946_1_PhyITCMSize),
    .ITCMRData                (pxGen_a946_1_ITCMRData),
    .GateTheCLK               (pxGen_a946_1_GateTheCLK),
    .HADDR                    (pxGen_a946_1_HADDR),
    .HBURST                   (pxGen_a946_1_HBURST),
    .HLOCK                    (pxGen_a946_1_HLOCK),
    .HPROT                    (pxGen_a946_1_HPROT),
    .HBUSREQ                  (pxGen_a946_1_HBUSREQ),
    .HSIZE                    (pxGen_a946_1_HSIZE),
    .HTRANS                   (pxGen_a946_1_HTRANS),
    .HWDATA                   (pxGen_a946_1_HWDATA),
    .HWRITE                   (pxGen_a946_1_HWRITE),
    .BIGENDOUT                (pxGen_a946_1_BIGENDOUT),
    .COMMRX                   (pxGen_a946_1_COMMRX),
    .COMMTX                   (pxGen_a946_1_COMMTX),
    .nCPMREQ                  (pxGen_a946_1_nCPMREQ),
    .nCPTRANS                 (pxGen_a946_1_nCPTRANS),
    .CPCLKEN                  (pxGen_a946_1_CPCLKEN),
    .CPDOUT                   (pxGen_a946_1_CPDOUT),
    .CPINSTR                  (pxGen_a946_1_CPINSTR),
    .CPLATECANCEL             (pxGen_a946_1_CPLATECANCEL),
    .CPPASS                   (pxGen_a946_1_CPPASS),
    .CPTBIT                   (pxGen_a946_1_CPTBIT),
    .SO                       (pxGen_a946_1_SO),
    .DBGACK                   (pxGen_a946_1_DBGACK),
    .DBGINSTREXEC             (pxGen_a946_1_DBGINSTREXEC),
    .DBGIR                    (pxGen_a946_1_DBGIR),
    .DBGRNG                   (pxGen_a946_1_DBGRNG),
    .DBGRQI                   (pxGen_a946_1_DBGRQI),
    .DBGSCREG                 (pxGen_a946_1_DBGSCREG),
    .DBGSDIN                  (pxGen_a946_1_DBGSDIN),
    .DBGTAPSM                 (pxGen_a946_1_DBGTAPSM),
    .DBGTDO                   (pxGen_a946_1_DBGTDO),
    .DBGnTDOEN                (pxGen_a946_1_DBGnTDOEN),
    .ETMBIGEND                (pxGen_a946_1_ETMBIGEND),
    .ETMCHSD                  (pxGen_a946_1_ETMCHSD),
    .ETMCHSE                  (pxGen_a946_1_ETMCHSE),
    .ETMDABORT                (pxGen_a946_1_ETMDABORT),
    .ETMDA                    (pxGen_a946_1_ETMDA),
    .ETMDBGACK                (pxGen_a946_1_ETMDBGACK),
    .ETMDMAS                  (pxGen_a946_1_ETMDMAS),
    .ETMDMORE                 (pxGen_a946_1_ETMDMORE),
    .ETMDSEQ                  (pxGen_a946_1_ETMDSEQ),
    .ETMDnMREQ                (pxGen_a946_1_ETMDnMREQ),
    .ETMDnRW                  (pxGen_a946_1_ETMDnRW),
    .ETMHIVECS                (pxGen_a946_1_ETMHIVECS),
    .ETMIABORT                (pxGen_a946_1_ETMIABORT),
    .ETMIA                    (pxGen_a946_1_ETMIA),
    .ETMID31To25              (pxGen_a946_1_ETMID31To25),
    .ETMID15To11              (pxGen_a946_1_ETMID15To11),
    .ETMINSTREXEC             (pxGen_a946_1_ETMINSTREXEC),
    .ETMISEQ                  (pxGen_a946_1_ETMISEQ),
    .ETMITBIT                 (pxGen_a946_1_ETMITBIT),
    .ETMInMREQ                (pxGen_a946_1_ETMInMREQ),
    .ETMLATECANCEL            (pxGen_a946_1_ETMLATECANCEL),
    .ETMPASS                  (pxGen_a946_1_ETMPASS),
    .ETMPROCID                (pxGen_a946_1_ETMPROCID),
    .ETMPROCIDWR              (pxGen_a946_1_ETMPROCIDWR),
    .ETMINSTRVALID            (pxGen_a946_1_ETMINSTRVALID),
    .ETMRDATA                 (pxGen_a946_1_ETMRDATA),
    .ETMRNGOUT                (pxGen_a946_1_ETMRNGOUT),
    .ETMWDATA                 (pxGen_a946_1_ETMWDATA),
    .ETMnWAIT                 (pxGen_a946_1_ETMnWAIT),
    .DTCMWData                (pxGen_a946_1_DTCMWData),
    .DTCMWEn                  (pxGen_a946_1_DTCMWEn),
    .DTCMEn                   (pxGen_a946_1_DTCMEn),
    .DTCMAdrs                 (pxGen_a946_1_DTCMAdrs),
    .ITCMWData                (pxGen_a946_1_ITCMWData),
    .ITCMWEn                  (pxGen_a946_1_ITCMWEn),
    .ITCMEn                   (pxGen_a946_1_ITCMEn),
    .ITCMAdrs                 (pxGen_a946_1_ITCMAdrs) 
    );
// 
// Component: AhbMemory_1 Instantiation
// 

// 
//  Default value assignement for signals
// 
    //  defparam AhbMemory_1.numAddrBits=22;
    //  defparam AhbMemory_1.waitStates=0;
AhbMemory #(22,0) AhbMemory_1(
    .HCLK                     (pxGen_AhbMemory_1_HCLK),
    .HRESETn                  (pxGen_AhbMemory_1_HRESETn),
    .HADDR                    (pxGen_AhbMemory_1_HADDR),
    .HTRANS                   (pxGen_AhbMemory_1_HTRANS),
    .HWRITE                   (pxGen_AhbMemory_1_HWRITE),
    .HSIZE                    (pxGen_AhbMemory_1_HSIZE),
    .HWDATA                   (pxGen_AhbMemory_1_HWDATA),
    .HSEL                     (pxGen_AhbMemory_1_HSEL),
    .HREADYin                 (pxGen_AhbMemory_1_HREADYin),
    .HRDATA                   (pxGen_AhbMemory_1_HRDATA),
    .HREADYout                (pxGen_AhbMemory_1_HREADYout),
    .HRESP                    (pxGen_AhbMemory_1_HRESP) 
    );
// 
// Component: AhbMemory_2 Instantiation
// 

// 
//  Default value assignement for signals
// 
    //  defparam AhbMemory_2.numAddrBits=21;
    //  defparam AhbMemory_2.waitStates=0;
AhbMemory #(21,0) AhbMemory_2(
    .HCLK                     (pxGen_AhbMemory_2_HCLK),
    .HRESETn                  (pxGen_AhbMemory_2_HRESETn),
    .HADDR                    (pxGen_AhbMemory_2_HADDR),
    .HTRANS                   (pxGen_AhbMemory_2_HTRANS),
    .HWRITE                   (pxGen_AhbMemory_2_HWRITE),
    .HSIZE                    (pxGen_AhbMemory_2_HSIZE),
    .HWDATA                   (pxGen_AhbMemory_2_HWDATA),
    .HSEL                     (pxGen_AhbMemory_2_HSEL),
    .HREADYin                 (pxGen_AhbMemory_2_HREADYin),
    .HRDATA                   (pxGen_AhbMemory_2_HRDATA),
    .HREADYout                (pxGen_AhbMemory_2_HREADYout),
    .HRESP                    (pxGen_AhbMemory_2_HRESP) 
    );
// 
// Decoder: PxDecoder_AHB_1_1407810334 Instantiation
// 

// 
//  Default value assignement for signals
// 
PxDecoder_AHB_1_1407810334_module  PxDecoder_AHB_1_1407810334(
    .a946_1_CLK               (pxGen_a946_1_CLK),
    .AhbMemory_1_HCLK         (pxGen_AhbMemory_1_HCLK),
    .AhbMemory_2_HCLK         (pxGen_AhbMemory_2_HCLK),
    .a946_1_HRESETn           (pxGen_a946_1_HRESETn),
    .AhbMemory_1_HRESETn      (pxGen_AhbMemory_1_HRESETn),
    .AhbMemory_2_HRESETn      (pxGen_AhbMemory_2_HRESETn),
    .a946_1_HADDR             (pxGen_a946_1_HADDR),
    .AhbMemory_1_HADDR        (pxGen_AhbMemory_1_HADDR),
    .AhbMemory_2_HADDR        (pxGen_AhbMemory_2_HADDR),
    .a946_1_HTRANS            (pxGen_a946_1_HTRANS),
    .AhbMemory_1_HTRANS       (pxGen_AhbMemory_1_HTRANS),
    .AhbMemory_2_HTRANS       (pxGen_AhbMemory_2_HTRANS),
    .a946_1_HWRITE            (pxGen_a946_1_HWRITE),
    .AhbMemory_1_HWRITE       (pxGen_AhbMemory_1_HWRITE),
    .AhbMemory_2_HWRITE       (pxGen_AhbMemory_2_HWRITE),
    .a946_1_HSIZE             (pxGen_a946_1_HSIZE),
    .AhbMemory_1_HSIZE        (pxGen_AhbMemory_1_HSIZE),
    .AhbMemory_2_HSIZE        (pxGen_AhbMemory_2_HSIZE),
    .a946_1_HBURST            (pxGen_a946_1_HBURST),
    .a946_1_HPROT             (pxGen_a946_1_HPROT),
    .a946_1_HWDATA            (pxGen_a946_1_HWDATA),
    .AhbMemory_1_HWDATA       (pxGen_AhbMemory_1_HWDATA),
    .AhbMemory_2_HWDATA       (pxGen_AhbMemory_2_HWDATA),
    .AhbMemory_1_HSEL         (pxGen_AhbMemory_1_HSEL),
    .AhbMemory_2_HSEL         (pxGen_AhbMemory_2_HSEL),
    .a946_1_HLOCK             (pxGen_a946_1_HLOCK),
    .a946_1_HRDATA            (pxGen_a946_1_HRDATA),
    .AhbMemory_1_HRDATA       (pxGen_AhbMemory_1_HRDATA),
    .AhbMemory_2_HRDATA       (pxGen_AhbMemory_2_HRDATA),
    .AhbMemory_1_HREADYin     (pxGen_AhbMemory_1_HREADYin),
    .AhbMemory_2_HREADYin     (pxGen_AhbMemory_2_HREADYin),
    .a946_1_HREADY            (pxGen_a946_1_HREADY),
    .AhbMemory_1_HREADYout    (pxGen_AhbMemory_1_HREADYout),
    .AhbMemory_2_HREADYout    (pxGen_AhbMemory_2_HREADYout),
    .a946_1_HRESP             (pxGen_a946_1_HRESP),
    .AhbMemory_1_HRESP        (pxGen_AhbMemory_1_HRESP),
    .AhbMemory_2_HRESP        (pxGen_AhbMemory_2_HRESP),
    .a946_1_HBUSREQ           (pxGen_a946_1_HBUSREQ),
    .a946_1_HGRANT            (pxGen_a946_1_HGRANT),
    .AHB_1_ambaAHB_HCLK       (test3_AHB_1_ambaAHB_HCLK),
    .AHB_1_ambaAHB_HRESETn    (test3_AHB_1_ambaAHB_HRESETn) 
    );


////////////////////
// Channel Bus Interface Connections
////////////////////






endmodule
