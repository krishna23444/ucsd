--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: full_adder_timesim.vhd
-- /___/   /\     Timestamp: Mon Apr 18 17:53:46 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf full_adder.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim full_adder.ncd full_adder_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: full_adder.ncd
-- Output file	: full_adder_timesim.vhd
-- # of Entities	: 1
-- Design Name	: full_adder
-- Xilinx	: G:/xilinx/ise71
--             
-- Purpose:    
--     This VHDL netlist is a verification model and uses simulation 
--     primitives which may not represent the true implementation of the 
--     device, however the netlist is functionally correct and should not 
--     be modified. This file cannot be synthesized and should only be used 
--     with supported simulation tools.
--             
-- Reference:  
--     Development System Reference Guide, Chapter 23
--     Synthesis and Verification Design Guide, Chapter 6
--             
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

entity full_adder is
  port (
    Ci : in STD_LOGIC := 'X'; 
    A : in STD_LOGIC := 'X'; 
    B : in STD_LOGIC := 'X'; 
    Co : out STD_LOGIC; 
    S : out STD_LOGIC 
  );
end full_adder;

architecture Structure of full_adder is
  signal Ci_IBUF : STD_LOGIC; 
  signal A_IBUF : STD_LOGIC; 
  signal B_IBUF : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal Ci_INBUF : STD_LOGIC; 
  signal Co_ENABLE : STD_LOGIC; 
  signal Co_O : STD_LOGIC; 
  signal A_INBUF : STD_LOGIC; 
  signal B_INBUF : STD_LOGIC; 
  signal S_ENABLE : STD_LOGIC; 
  signal S_O : STD_LOGIC; 
  signal S_OBUF : STD_LOGIC; 
  signal Co_OBUF : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
begin
  Ci_IBUF_0 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => Ci,
      O => Ci_INBUF
    );
  Ci_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => Ci_INBUF,
      O => Ci_IBUF
    );
  Co_OBUF_1 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => Co_O,
      CTL => Co_ENABLE,
      O => Co
    );
  Co_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => Co_ENABLE
    );
  A_IBUF_2 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A,
      O => A_INBUF
    );
  A_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_INBUF,
      O => A_IBUF
    );
  B_IBUF_3 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B,
      O => B_INBUF
    );
  B_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_INBUF,
      O => B_IBUF
    );
  S_OBUF_4 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_O,
      CTL => S_ENABLE,
      O => S
    );
  S_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_ENABLE
    );
  XLXI_3 : X_LUT4
    generic map(
      INIT => X"E8E8"
    )
    port map (
      ADR0 => Ci_IBUF,
      ADR1 => B_IBUF,
      ADR2 => A_IBUF,
      ADR3 => VCC,
      O => Co_OBUF
    );
  XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"9696"
    )
    port map (
      ADR0 => Ci_IBUF,
      ADR1 => B_IBUF,
      ADR2 => A_IBUF,
      ADR3 => VCC,
      O => S_OBUF
    );
  Co_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => Co_OBUF,
      O => Co_O
    );
  S_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_OBUF,
      O => S_O
    );
  NlwBlock_full_adder_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

