--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: fourbit_adder_timesim.vhd
-- /___/   /\     Timestamp: Mon Apr 18 20:57:39 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf fourbit_adder.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim fourbit_adder.ncd fourbit_adder_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: fourbit_adder.ncd
-- Output file	: fourbit_adder_timesim.vhd
-- # of Entities	: 1
-- Design Name	: fourbit_adder
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

entity fourbit_adder is
  port (
    Ci : in STD_LOGIC := 'X'; 
    Co : out STD_LOGIC; 
    A : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    B : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    S : out STD_LOGIC_VECTOR ( 3 downto 0 ) 
  );
end fourbit_adder;

architecture Structure of fourbit_adder is
  signal A_0_IBUF : STD_LOGIC; 
  signal A_1_IBUF : STD_LOGIC; 
  signal A_2_IBUF : STD_LOGIC; 
  signal A_3_IBUF : STD_LOGIC; 
  signal B_0_IBUF : STD_LOGIC; 
  signal B_1_IBUF : STD_LOGIC; 
  signal B_2_IBUF : STD_LOGIC; 
  signal B_3_IBUF : STD_LOGIC; 
  signal Ci_IBUF : STD_LOGIC; 
  signal XLXI_3_XLXN_5_0 : STD_LOGIC; 
  signal XLXN_3 : STD_LOGIC; 
  signal XLXI_4_XLXN_5_0 : STD_LOGIC; 
  signal XLXN_7_0 : STD_LOGIC; 
  signal XLXN_10 : STD_LOGIC; 
  signal XLXI_2_XLXN_5 : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal A_0_INBUF : STD_LOGIC; 
  signal A_1_INBUF : STD_LOGIC; 
  signal A_2_INBUF : STD_LOGIC; 
  signal A_3_INBUF : STD_LOGIC; 
  signal B_0_INBUF : STD_LOGIC; 
  signal B_1_INBUF : STD_LOGIC; 
  signal B_2_INBUF : STD_LOGIC; 
  signal B_3_INBUF : STD_LOGIC; 
  signal Ci_INBUF : STD_LOGIC; 
  signal Co_ENABLE : STD_LOGIC; 
  signal Co_O : STD_LOGIC; 
  signal S_0_ENABLE : STD_LOGIC; 
  signal S_0_O : STD_LOGIC; 
  signal S_1_ENABLE : STD_LOGIC; 
  signal S_1_O : STD_LOGIC; 
  signal S_2_ENABLE : STD_LOGIC; 
  signal S_2_O : STD_LOGIC; 
  signal S_3_ENABLE : STD_LOGIC; 
  signal S_3_O : STD_LOGIC; 
  signal S_2_OBUF : STD_LOGIC; 
  signal XLXI_3_XLXN_5 : STD_LOGIC; 
  signal S_3_OBUF : STD_LOGIC; 
  signal Co_OBUF : STD_LOGIC; 
  signal XLXN_10_pack_1 : STD_LOGIC; 
  signal S_1_OBUF : STD_LOGIC; 
  signal XLXI_2_XLXN_5_pack_1 : STD_LOGIC; 
  signal S_0_OBUF : STD_LOGIC; 
  signal XLXN_7 : STD_LOGIC; 
  signal XLXN_3_pack_1 : STD_LOGIC; 
  signal XLXI_4_XLXN_5 : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
begin
  A_0_IBUF_0 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(0),
      O => A_0_INBUF
    );
  A_0_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_0_INBUF,
      O => A_0_IBUF
    );
  A_1_IBUF_1 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(1),
      O => A_1_INBUF
    );
  A_1_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_1_INBUF,
      O => A_1_IBUF
    );
  A_2_IBUF_2 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(2),
      O => A_2_INBUF
    );
  A_2_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_2_INBUF,
      O => A_2_IBUF
    );
  A_3_IBUF_3 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(3),
      O => A_3_INBUF
    );
  A_3_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_3_INBUF,
      O => A_3_IBUF
    );
  B_0_IBUF_4 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(0),
      O => B_0_INBUF
    );
  B_0_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_0_INBUF,
      O => B_0_IBUF
    );
  B_1_IBUF_5 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(1),
      O => B_1_INBUF
    );
  B_1_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_1_INBUF,
      O => B_1_IBUF
    );
  B_2_IBUF_6 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(2),
      O => B_2_INBUF
    );
  B_2_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_2_INBUF,
      O => B_2_IBUF
    );
  B_3_IBUF_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(3),
      O => B_3_INBUF
    );
  B_3_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_3_INBUF,
      O => B_3_IBUF
    );
  Ci_IBUF_8 : X_BUF_PP
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
  Co_OBUF_9 : X_TRI_PP
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
  S_0_OBUF_10 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_0_O,
      CTL => S_0_ENABLE,
      O => S(0)
    );
  S_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_0_ENABLE
    );
  S_1_OBUF_11 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_1_O,
      CTL => S_1_ENABLE,
      O => S(1)
    );
  S_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_1_ENABLE
    );
  S_2_OBUF_12 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_2_O,
      CTL => S_2_ENABLE,
      O => S(2)
    );
  S_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_2_ENABLE
    );
  S_3_OBUF_13 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_3_O,
      CTL => S_3_ENABLE,
      O => S(3)
    );
  S_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_3_ENABLE
    );
  XLXI_3_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"1E78"
    )
    port map (
      ADR0 => A_1_IBUF,
      ADR1 => XLXN_3,
      ADR2 => XLXI_3_XLXN_5_0,
      ADR3 => B_1_IBUF,
      O => S_2_OBUF
    );
  XLXI_3_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXN_5,
      O => XLXI_3_XLXN_5_0
    );
  XLXI_4_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"1E78"
    )
    port map (
      ADR0 => XLXN_7_0,
      ADR1 => A_2_IBUF,
      ADR2 => XLXI_4_XLXN_5_0,
      ADR3 => B_2_IBUF,
      O => S_3_OBUF
    );
  Co_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_10_pack_1,
      O => XLXN_10
    );
  XLXI_3_XLXI_3 : X_LUT4
    generic map(
      INIT => X"E8E8"
    )
    port map (
      ADR0 => B_2_IBUF,
      ADR1 => XLXN_7_0,
      ADR2 => A_2_IBUF,
      ADR3 => VCC,
      O => XLXN_10_pack_1
    );
  S_1_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_5_pack_1,
      O => XLXI_2_XLXN_5
    );
  XLXI_2_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"55AA"
    )
    port map (
      ADR0 => B_1_IBUF,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => A_1_IBUF,
      O => XLXI_2_XLXN_5_pack_1
    );
  XLXI_1_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"9966"
    )
    port map (
      ADR0 => Ci_IBUF,
      ADR1 => A_0_IBUF,
      ADR2 => VCC,
      ADR3 => B_0_IBUF,
      O => S_0_OBUF
    );
  XLXN_7_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_7,
      O => XLXN_7_0
    );
  XLXN_7_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_3_pack_1,
      O => XLXN_3
    );
  XLXI_1_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => B_0_IBUF,
      ADR1 => VCC,
      ADR2 => A_0_IBUF,
      ADR3 => Ci_IBUF,
      O => XLXN_3_pack_1
    );
  XLXI_4_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXN_5,
      O => XLXI_4_XLXN_5_0
    );
  XLXI_4_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"6666"
    )
    port map (
      ADR0 => A_3_IBUF,
      ADR1 => B_3_IBUF,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_4_XLXN_5
    );
  XLXI_3_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => A_2_IBUF,
      ADR2 => VCC,
      ADR3 => B_2_IBUF,
      O => XLXI_3_XLXN_5
    );
  XLXI_4_XLXI_3 : X_LUT4
    generic map(
      INIT => X"EE88"
    )
    port map (
      ADR0 => A_3_IBUF,
      ADR1 => B_3_IBUF,
      ADR2 => VCC,
      ADR3 => XLXN_10,
      O => Co_OBUF
    );
  XLXI_2_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"17E8"
    )
    port map (
      ADR0 => Ci_IBUF,
      ADR1 => A_0_IBUF,
      ADR2 => B_0_IBUF,
      ADR3 => XLXI_2_XLXN_5,
      O => S_1_OBUF
    );
  XLXI_2_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => A_1_IBUF,
      ADR1 => VCC,
      ADR2 => B_1_IBUF,
      ADR3 => XLXN_3,
      O => XLXN_7
    );
  Co_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => Co_OBUF,
      O => Co_O
    );
  S_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_0_OBUF,
      O => S_0_O
    );
  S_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_1_OBUF,
      O => S_1_O
    );
  S_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_2_OBUF,
      O => S_2_O
    );
  S_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_3_OBUF,
      O => S_3_O
    );
  NlwBlock_fourbit_adder_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

