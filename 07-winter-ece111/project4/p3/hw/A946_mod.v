`timescale 1ns/1ps

module a946 (CLK,
  UnGatedCLK,
  GateTheCLK,
  HCLKEN,
  HGRANT,
  HRDATA,
  HREADY,
  HRESETn,
  HRESP,
  HADDR,
  HBURST,
  HLOCK,
  HPROT,
  HBUSREQ,
  HSIZE,
  HTRANS,
  HWDATA,
  HWRITE,

//  Miscellaneous Signals
  nFIQ,
  nIRQ,
  TAPID,
  VINITHI,
  INITRAM,
  DCacheSize,
  ICacheSize,
  BIGENDOUT,
  
// Debug Signals
  COMMRX,
  COMMTX,

//Coprocessor Interface Signals
  CHSDE,
  CHSEX,
  CPDIN,
  nCPMREQ,
  nCPTRANS,
  CPCLKEN,
  CPDOUT,
  CPINSTR,
  CPLATECANCEL,
  CPPASS,
  CPTBIT,

  DBGDEWPT,
  DBGEN,
  DBGEXT,
  DBGIEBKPT,
  DBGSDOUT,
  DBGTCKEN, //*
  DBGTDI,
  DBGTMS,
  DBGnTRST,

// Test Wrapper Signals
  SI,
  SCANEN,
  TESTMODE,
  SERIALEN,
  TESTEN,
  INnotEXTEST,
  ETMFIFOFULL,
  SO,
  DBGACK,
  DBGINSTREXEC,
  DBGIR,
  DBGRNG,
  DBGRQI,
  DBGSCREG,
  DBGSDIN,

  DBGTAPSM,
  DBGTDO,
  DBGnTDOEN,
  EDBGRQ,

//ETM Interface Signals
  ETMEN,
  ETMBIGEND,
  ETMCHSD,
  ETMCHSE,
  ETMDABORT,
  ETMDA,
  ETMDBGACK,
  ETMDMAS,
  ETMDMORE,
  ETMDSEQ,
  ETMDnMREQ,
  ETMDnRW,
  ETMHIVECS,
  ETMIABORT,
  ETMIA,
  ETMID31To25,
  ETMID15To11,
  ETMINSTREXEC,
  ETMISEQ,
  ETMITBIT,
  ETMInMREQ,
  ETMLATECANCEL,
  ETMPASS,
  ETMPROCID,
  ETMPROCIDWR,
  ETMINSTRVALID,
  ETMRDATA,
  ETMRNGOUT,
  ETMWDATA,
  ETMnWAIT,

// TCM  interface signals
  PhyDTCMSize,
  DTCMRData,
  DTCMWData,
  DTCMWEn,
  DTCMEn,
  DTCMAdrs,
  PhyITCMSize,
  ITCMRData,
  ITCMWData,
  ITCMWEn,
  ITCMEn,
  ITCMAdrs
  );

  //----------------------------------------------------------------------
  // parameters
  //----------------------------------------------------------------------
parameter
  DEFAULT_PIN_DELAY = 5,
  PIN_DELAY_FILE = "$CVE_HOME/delays/arm946e_sr1_delays",
  CVE_NAME = "";

  //----------------------------------------------------------------------
  // I/O signals
  //----------------------------------------------------------------------

input CLK;
input UnGatedCLK;
output GateTheCLK;
input HCLKEN;
input HGRANT;
input [31:0] HRDATA;
input HREADY;
input HRESETn;
input [1:0] HRESP;
output [31:0] HADDR;
output [2:0] HBURST;
output HLOCK;
output [3:0] HPROT;
output HBUSREQ;
output [2:0] HSIZE;
output [1:0] HTRANS;
output [31:0] HWDATA;
output HWRITE;

input nFIQ;
input nIRQ;
input [31:0] TAPID;
input INITRAM;
input VINITHI;
input [3:0] DCacheSize;
input [3:0] ICacheSize;
output BIGENDOUT;
output COMMRX;
output COMMTX;
input [1:0] CHSDE;
input [1:0] CHSEX;
input [31:0] CPDIN;
output nCPMREQ;
output nCPTRANS;
output CPCLKEN;
output [31:0] CPDOUT;
output [31:0] CPINSTR;
output CPLATECANCEL;
output CPPASS;
output CPTBIT;
input DBGDEWPT;
input DBGEN;
input [1:0] DBGEXT;
input DBGIEBKPT;
input DBGSDOUT;
input DBGTCKEN;  //*
input DBGTDI;
input DBGTMS;
input DBGnTRST;
input SI;
input SCANEN;
input TESTMODE;
input SERIALEN;
input TESTEN;
input INnotEXTEST;
input ETMFIFOFULL;
output SO;
output DBGACK;
output DBGINSTREXEC;
output [3:0] DBGIR;
output [1:0] DBGRNG;
output DBGRQI;
output [4:0] DBGSCREG;
output DBGSDIN;
output [3:0] DBGTAPSM;
output DBGTDO;
output DBGnTDOEN;
input EDBGRQ;

input ETMEN;
output ETMBIGEND;
output [1:0] ETMCHSD;
output [1:0] ETMCHSE;
output ETMDABORT;
output [31:0] ETMDA;
output ETMDBGACK;
output [1:0] ETMDMAS;
output ETMDMORE;
output ETMDSEQ;
output ETMDnMREQ;
output ETMDnRW;
output ETMHIVECS;
output ETMIABORT;
output [31:1] ETMIA;
output [31:25] ETMID31To25;
output [15:11] ETMID15To11;
output ETMINSTREXEC;
output ETMISEQ;
output ETMITBIT;
output ETMInMREQ;
output ETMLATECANCEL;
output ETMPASS;
output [31:0] ETMPROCID;
output ETMPROCIDWR;
output ETMINSTRVALID;
output [31:0] ETMRDATA;
output [1:0] ETMRNGOUT;
output [31:0] ETMWDATA;
output ETMnWAIT;

input [3:0]  PhyDTCMSize;
input [31:0] DTCMRData;
output[31:0] DTCMWData;
output[3:0]  DTCMWEn;
output       DTCMEn;
output[17:0] DTCMAdrs;
input [3:0]  PhyITCMSize;
input [31:0] ITCMRData;
output[31:0] ITCMWData;
output[3:0] ITCMWEn;
output      ITCMEn;
output[17:0] ITCMAdrs;

wire [31:0] HRDATA_D;
wire [1:0] HRESP_D;
wire HREADY_D;
wire HGRANT_D;

assign #3 HRDATA_D = HRDATA;
assign #3 HREADY_D = HREADY;
assign #3 HRESP_D = HRESP;
assign #3 HGRANT_D = HGRANT;

ARM946E_Sr1 uARM946E_Sr1(
  .CLK (CLK),
  .UnGatedCLK (UnGatedCLK),
  .GateTheCLK (GateTheCLK),
  .HCLKEN (HCLKEN),
  .HGRANT (HGRANT_D),
  .HRDATA (HRDATA_D[31:0]),
  .HREADY (HREADY_D),
  .HRESETn (HRESETn),
  .HRESP (HRESP_D[1:0]),
  .HADDR (HADDR[31:0]),
  .HBURST (HBURST[2:0]),
  .HLOCK (HLOCK),
  .HPROT (HPROT[3:0]),
  .HBUSREQ (HBUSREQ),
  .HSIZE (HSIZE[2:0]),
  .HTRANS (HTRANS[1:0]),
  .HWDATA (HWDATA[31:0]),
  .HWRITE (HWRITE),

  .nFIQ (nFIQ),
  .nIRQ (nIRQ),
  .TAPID (TAPID[31:0]),
  .VINITHI (VINITHI),
  .INITRAM (INITRAM),
  .DCacheSize (DCacheSize),
  .ICacheSize (ICacheSize),
  .BIGENDOUT (BIGENDOUT),

  .COMMRX (COMMRX),
  .COMMTX (COMMTX),
  .CHSDE (CHSDE[1:0]),
  .CHSEX (CHSEX[1:0]),
  .CPDIN (CPDIN[31:0]),
  .nCPMREQ (nCPMREQ),
  .nCPTRANS (nCPTRANS),
  .CPCLKEN (CPCLKEN),
  .CPDOUT (CPDOUT[31:0]),
  .CPINSTR (CPINSTR[31:0]),
  .CPLATECANCEL (CPLATECANCEL),
  .CPPASS (CPPASS),
  .CPTBIT (CPTBIT),

  .DBGDEWPT (DBGDEWPT),
  .DBGEN (DBGEN),
  .DBGEXT (DBGEXT[1:0]),
  .DBGIEBKPT (DBGIEBKPT),
  .DBGSDOUT (DBGSDOUT),
  .DBGTCKEN (DBGTCKEN),
  .DBGTDI (DBGTDI),
  .DBGTMS (DBGTMS),
  .DBGnTRST (DBGnTRST),

  .SI (SI),
  .SCANEN (SCANEN),
  .TESTMODE (TESTMODE),
  .SERIALEN (SERIALEN),
  .TESTEN (TESTEN),
  .INnotEXTEST (INnotEXTEST),
  .ETMFIFOFULL (ETMFIFOFULL),
  .SO (SO),

  .DBGACK (DBGACK),
  .DBGINSTREXEC (DBGINSTREXEC),
  .DBGIR (DBGIR[3:0]),
  .DBGRNG (DBGRNG[1:0]),
  .DBGRQI (DBGRQI),
  .DBGSCREG (DBGSCREG[4:0]),
  .DBGSDIN (DBGSDIN),
  .DBGTAPSM (DBGTAPSM[3:0]),
  .DBGTDO (DBGTDO),
  .DBGnTDOEN (DBGnTDOEN),
  .EDBGRQ (EDBGRQ),

  .ETMEN (ETMEN),
  .ETMBIGEND (ETMBIGEND),
  .ETMCHSD (ETMCHSD[1:0]), 
  .ETMCHSE (ETMCHSE[1:0]),
  .ETMDABORT (ETMDABORT),
  .ETMDA (ETMDA[31:0]),
  .ETMDBGACK (ETMDBGACK),
  .ETMDMAS (ETMDMAS[1:0]),
  .ETMDMORE (ETMDMORE),
  .ETMDSEQ (ETMDSEQ),
  .ETMDnMREQ (ETMDnMREQ),
  .ETMDnRW (ETMDnRW), 
  .ETMHIVECS (ETMHIVECS),
  .ETMIABORT (ETMIABORT),
  .ETMIA (ETMIA[31:1]),
  .ETMID15To11 (ETMID15To11[15:11]),
  .ETMID31To25 (ETMID31To25[31:25]),
  .ETMINSTREXEC (ETMINSTREXEC),
  .ETMISEQ (ETMISEQ),
  .ETMITBIT (ETMITBIT),
  .ETMInMREQ (ETMInMREQ),
  .ETMLATECANCEL (ETMLATECANCEL),
  .ETMPASS (ETMPASS),
  .ETMPROCID (ETMPROCID),
  .ETMPROCIDWR (TMPROCIDWR),
  .ETMINSTRVALID (ETMINSTRVALID),
  .ETMRDATA (ETMRDATA[31:0]),
  .ETMRNGOUT (ETMRNGOUT[1:0]),
  .ETMWDATA (ETMWDATA[31:0]),
  .ETMnWAIT (ETMnWAIT),

  .PhyDTCMSize (PhyDTCMSize),
  .DTCMRData (DTCMRData),
  .DTCMWData (DTCMWData),
  .DTCMWEn (DTCMWEn),
  .DTCMEn (DTCMEn),
  .DTCMAdrs (DTCMAdrs),
  .PhyITCMSize (PhyITCMSize),
  .ITCMRData (ITCMRData),
  .ITCMWData (ITCMWData),
  .ITCMWEn (ITCMWEn),
  .ITCMEn (ITCMEn),
  .ITCMAdrs (ITCMAdrs)
  );

endmodule

