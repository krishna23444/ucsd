--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: sixteenbit_adder_timesim.vhd
-- /___/   /\     Timestamp: Mon Apr 18 21:16:16 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf sixteenbit_adder.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim sixteenbit_adder.ncd sixteenbit_adder_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: sixteenbit_adder.ncd
-- Output file	: sixteenbit_adder_timesim.vhd
-- # of Entities	: 1
-- Design Name	: sixteenbit_adder
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

entity sixteenbit_adder is
  port (
    Ci : in STD_LOGIC := 'X'; 
    Co : out STD_LOGIC; 
    A : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    B : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    S : out STD_LOGIC_VECTOR ( 15 downto 0 ) 
  );
end sixteenbit_adder;

architecture Structure of sixteenbit_adder is
  signal A_0_IBUF : STD_LOGIC; 
  signal A_1_IBUF : STD_LOGIC; 
  signal A_2_IBUF : STD_LOGIC; 
  signal A_3_IBUF : STD_LOGIC; 
  signal B_0_IBUF : STD_LOGIC; 
  signal A_4_IBUF : STD_LOGIC; 
  signal B_1_IBUF : STD_LOGIC; 
  signal A_5_IBUF : STD_LOGIC; 
  signal B_2_IBUF : STD_LOGIC; 
  signal A_6_IBUF : STD_LOGIC; 
  signal B_3_IBUF : STD_LOGIC; 
  signal A_7_IBUF : STD_LOGIC; 
  signal B_4_IBUF : STD_LOGIC; 
  signal A_8_IBUF : STD_LOGIC; 
  signal B_5_IBUF : STD_LOGIC; 
  signal A_9_IBUF : STD_LOGIC; 
  signal B_6_IBUF : STD_LOGIC; 
  signal B_7_IBUF : STD_LOGIC; 
  signal B_8_IBUF : STD_LOGIC; 
  signal B_9_IBUF : STD_LOGIC; 
  signal Ci_IBUF : STD_LOGIC; 
  signal A_10_IBUF : STD_LOGIC; 
  signal A_11_IBUF : STD_LOGIC; 
  signal A_12_IBUF : STD_LOGIC; 
  signal A_13_IBUF : STD_LOGIC; 
  signal A_14_IBUF : STD_LOGIC; 
  signal A_15_IBUF : STD_LOGIC; 
  signal B_10_IBUF : STD_LOGIC; 
  signal B_11_IBUF : STD_LOGIC; 
  signal B_12_IBUF : STD_LOGIC; 
  signal B_13_IBUF : STD_LOGIC; 
  signal B_14_IBUF : STD_LOGIC; 
  signal B_15_IBUF : STD_LOGIC; 
  signal XLXI_4_XLXN_3 : STD_LOGIC; 
  signal XLXN_2_0 : STD_LOGIC; 
  signal XLXI_4_XLXN_7_0 : STD_LOGIC; 
  signal XLXI_4_XLXI_2_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_4_XLXI_1_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_3_XLXI_4_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_3_XLXN_7_0 : STD_LOGIC; 
  signal XLXI_3_XLXI_3_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_2_XLXN_10 : STD_LOGIC; 
  signal XLXI_2_XLXN_7_0 : STD_LOGIC; 
  signal XLXN_1_0 : STD_LOGIC; 
  signal XLXI_5_XLXI_3_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_5_XLXN_3 : STD_LOGIC; 
  signal XLXI_5_XLXI_2_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_3_XLXI_2_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_3_XLXI_1_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_5_XLXI_1_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_4_XLXN_10 : STD_LOGIC; 
  signal XLXI_4_XLXI_4_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_2_XLXI_4_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_2_XLXI_3_XLXN_5_0 : STD_LOGIC; 
  signal XLXN_3_0 : STD_LOGIC; 
  signal XLXI_5_XLXN_7_0 : STD_LOGIC; 
  signal XLXI_4_XLXI_3_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_3_XLXN_10 : STD_LOGIC; 
  signal XLXI_5_XLXI_4_XLXN_5_0 : STD_LOGIC; 
  signal XLXI_3_XLXN_3 : STD_LOGIC; 
  signal XLXI_2_XLXI_2_XLXN_5 : STD_LOGIC; 
  signal XLXI_2_XLXN_3 : STD_LOGIC; 
  signal XLXI_5_XLXN_10 : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal A_0_INBUF : STD_LOGIC; 
  signal A_1_INBUF : STD_LOGIC; 
  signal A_2_INBUF : STD_LOGIC; 
  signal A_3_INBUF : STD_LOGIC; 
  signal B_0_INBUF : STD_LOGIC; 
  signal A_4_INBUF : STD_LOGIC; 
  signal B_1_INBUF : STD_LOGIC; 
  signal A_5_INBUF : STD_LOGIC; 
  signal B_2_INBUF : STD_LOGIC; 
  signal A_6_INBUF : STD_LOGIC; 
  signal B_3_INBUF : STD_LOGIC; 
  signal A_7_INBUF : STD_LOGIC; 
  signal B_4_INBUF : STD_LOGIC; 
  signal A_8_INBUF : STD_LOGIC; 
  signal B_5_INBUF : STD_LOGIC; 
  signal A_9_INBUF : STD_LOGIC; 
  signal B_6_INBUF : STD_LOGIC; 
  signal B_7_INBUF : STD_LOGIC; 
  signal B_8_INBUF : STD_LOGIC; 
  signal B_9_INBUF : STD_LOGIC; 
  signal Ci_INBUF : STD_LOGIC; 
  signal S_10_ENABLE : STD_LOGIC; 
  signal S_10_O : STD_LOGIC; 
  signal S_11_ENABLE : STD_LOGIC; 
  signal S_11_O : STD_LOGIC; 
  signal Co_ENABLE : STD_LOGIC; 
  signal Co_O : STD_LOGIC; 
  signal S_12_ENABLE : STD_LOGIC; 
  signal S_12_O : STD_LOGIC; 
  signal S_13_ENABLE : STD_LOGIC; 
  signal S_13_O : STD_LOGIC; 
  signal S_14_ENABLE : STD_LOGIC; 
  signal S_14_O : STD_LOGIC; 
  signal S_15_ENABLE : STD_LOGIC; 
  signal S_15_O : STD_LOGIC; 
  signal S_0_ENABLE : STD_LOGIC; 
  signal S_0_O : STD_LOGIC; 
  signal S_1_ENABLE : STD_LOGIC; 
  signal S_1_O : STD_LOGIC; 
  signal S_2_ENABLE : STD_LOGIC; 
  signal S_2_O : STD_LOGIC; 
  signal A_10_INBUF : STD_LOGIC; 
  signal S_3_ENABLE : STD_LOGIC; 
  signal S_3_O : STD_LOGIC; 
  signal A_11_INBUF : STD_LOGIC; 
  signal S_4_ENABLE : STD_LOGIC; 
  signal S_4_O : STD_LOGIC; 
  signal A_12_INBUF : STD_LOGIC; 
  signal S_5_ENABLE : STD_LOGIC; 
  signal S_5_O : STD_LOGIC; 
  signal A_13_INBUF : STD_LOGIC; 
  signal S_6_ENABLE : STD_LOGIC; 
  signal S_6_O : STD_LOGIC; 
  signal A_14_INBUF : STD_LOGIC; 
  signal S_7_ENABLE : STD_LOGIC; 
  signal S_7_O : STD_LOGIC; 
  signal A_15_INBUF : STD_LOGIC; 
  signal S_8_ENABLE : STD_LOGIC; 
  signal S_8_O : STD_LOGIC; 
  signal S_9_ENABLE : STD_LOGIC; 
  signal S_9_O : STD_LOGIC; 
  signal B_10_INBUF : STD_LOGIC; 
  signal S_15_OBUF : STD_LOGIC; 
  signal XLXI_5_XLXI_3_XLXN_5 : STD_LOGIC; 
  signal S_6_OBUF : STD_LOGIC; 
  signal XLXI_3_XLXI_2_XLXN_5 : STD_LOGIC; 
  signal XLXI_5_XLXI_1_XLXN_5 : STD_LOGIC; 
  signal S_13_OBUF : STD_LOGIC; 
  signal XLXI_2_XLXI_4_XLXN_5 : STD_LOGIC; 
  signal S_4_OBUF : STD_LOGIC; 
  signal S_11_OBUF : STD_LOGIC; 
  signal XLXI_4_XLXI_3_XLXN_5 : STD_LOGIC; 
  signal S_1_OBUF : STD_LOGIC; 
  signal XLXI_2_XLXI_2_XLXN_5_pack_1 : STD_LOGIC; 
  signal XLXN_3 : STD_LOGIC; 
  signal XLXI_4_XLXN_10_pack_1 : STD_LOGIC; 
  signal S_0_OBUF : STD_LOGIC; 
  signal XLXI_2_XLXN_7 : STD_LOGIC; 
  signal XLXI_2_XLXN_3_pack_1 : STD_LOGIC; 
  signal XLXI_5_XLXI_4_XLXN_5 : STD_LOGIC; 
  signal Co_OBUF : STD_LOGIC; 
  signal XLXI_5_XLXN_10_pack_1 : STD_LOGIC; 
  signal XLXI_3_XLXN_7 : STD_LOGIC; 
  signal XLXI_3_XLXN_3_pack_1 : STD_LOGIC; 
  signal S_2_OBUF : STD_LOGIC; 
  signal B_11_INBUF : STD_LOGIC; 
  signal B_12_INBUF : STD_LOGIC; 
  signal B_13_INBUF : STD_LOGIC; 
  signal B_14_INBUF : STD_LOGIC; 
  signal B_15_INBUF : STD_LOGIC; 
  signal XLXI_4_XLXN_7 : STD_LOGIC; 
  signal XLXI_4_XLXN_3_pack_1 : STD_LOGIC; 
  signal S_9_OBUF : STD_LOGIC; 
  signal XLXI_4_XLXI_1_XLXN_5 : STD_LOGIC; 
  signal XLXI_3_XLXI_3_XLXN_5 : STD_LOGIC; 
  signal S_7_OBUF : STD_LOGIC; 
  signal XLXN_1 : STD_LOGIC; 
  signal XLXI_2_XLXN_10_pack_1 : STD_LOGIC; 
  signal S_14_OBUF : STD_LOGIC; 
  signal XLXI_5_XLXI_2_XLXN_5 : STD_LOGIC; 
  signal S_5_OBUF : STD_LOGIC; 
  signal XLXI_3_XLXI_1_XLXN_5 : STD_LOGIC; 
  signal XLXI_4_XLXI_4_XLXN_5 : STD_LOGIC; 
  signal S_12_OBUF : STD_LOGIC; 
  signal XLXI_2_XLXI_3_XLXN_5 : STD_LOGIC; 
  signal S_3_OBUF : STD_LOGIC; 
  signal XLXI_5_XLXN_7 : STD_LOGIC; 
  signal XLXI_5_XLXN_3_pack_1 : STD_LOGIC; 
  signal S_10_OBUF : STD_LOGIC; 
  signal XLXI_4_XLXI_2_XLXN_5 : STD_LOGIC; 
  signal XLXN_2 : STD_LOGIC; 
  signal XLXI_3_XLXN_10_pack_1 : STD_LOGIC; 
  signal XLXI_3_XLXI_4_XLXN_5 : STD_LOGIC; 
  signal S_8_OBUF : STD_LOGIC; 
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
  A_4_IBUF_5 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(4),
      O => A_4_INBUF
    );
  A_4_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_4_INBUF,
      O => A_4_IBUF
    );
  B_1_IBUF_6 : X_BUF_PP
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
  A_5_IBUF_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(5),
      O => A_5_INBUF
    );
  A_5_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_5_INBUF,
      O => A_5_IBUF
    );
  B_2_IBUF_8 : X_BUF_PP
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
  A_6_IBUF_9 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(6),
      O => A_6_INBUF
    );
  A_6_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_6_INBUF,
      O => A_6_IBUF
    );
  B_3_IBUF_10 : X_BUF_PP
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
  A_7_IBUF_11 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(7),
      O => A_7_INBUF
    );
  A_7_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_7_INBUF,
      O => A_7_IBUF
    );
  B_4_IBUF_12 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(4),
      O => B_4_INBUF
    );
  B_4_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_4_INBUF,
      O => B_4_IBUF
    );
  A_8_IBUF_13 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(8),
      O => A_8_INBUF
    );
  A_8_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_8_INBUF,
      O => A_8_IBUF
    );
  B_5_IBUF_14 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(5),
      O => B_5_INBUF
    );
  B_5_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_5_INBUF,
      O => B_5_IBUF
    );
  A_9_IBUF_15 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(9),
      O => A_9_INBUF
    );
  A_9_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_9_INBUF,
      O => A_9_IBUF
    );
  B_6_IBUF_16 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(6),
      O => B_6_INBUF
    );
  B_6_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_6_INBUF,
      O => B_6_IBUF
    );
  B_7_IBUF_17 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(7),
      O => B_7_INBUF
    );
  B_7_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_7_INBUF,
      O => B_7_IBUF
    );
  B_8_IBUF_18 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(8),
      O => B_8_INBUF
    );
  B_8_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_8_INBUF,
      O => B_8_IBUF
    );
  B_9_IBUF_19 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(9),
      O => B_9_INBUF
    );
  B_9_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_9_INBUF,
      O => B_9_IBUF
    );
  Ci_IBUF_20 : X_BUF_PP
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
  S_10_OBUF_21 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_10_O,
      CTL => S_10_ENABLE,
      O => S(10)
    );
  S_10_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_10_ENABLE
    );
  S_11_OBUF_22 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_11_O,
      CTL => S_11_ENABLE,
      O => S(11)
    );
  S_11_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_11_ENABLE
    );
  Co_OBUF_23 : X_TRI_PP
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
  S_12_OBUF_24 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_12_O,
      CTL => S_12_ENABLE,
      O => S(12)
    );
  S_12_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_12_ENABLE
    );
  S_13_OBUF_25 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_13_O,
      CTL => S_13_ENABLE,
      O => S(13)
    );
  S_13_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_13_ENABLE
    );
  S_14_OBUF_26 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_14_O,
      CTL => S_14_ENABLE,
      O => S(14)
    );
  S_14_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_14_ENABLE
    );
  S_15_OBUF_27 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_15_O,
      CTL => S_15_ENABLE,
      O => S(15)
    );
  S_15_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_15_ENABLE
    );
  S_0_OBUF_28 : X_TRI_PP
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
  S_1_OBUF_29 : X_TRI_PP
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
  S_2_OBUF_30 : X_TRI_PP
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
  A_10_IBUF_31 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(10),
      O => A_10_INBUF
    );
  A_10_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_10_INBUF,
      O => A_10_IBUF
    );
  S_3_OBUF_32 : X_TRI_PP
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
  A_11_IBUF_33 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(11),
      O => A_11_INBUF
    );
  A_11_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_11_INBUF,
      O => A_11_IBUF
    );
  S_4_OBUF_34 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_4_O,
      CTL => S_4_ENABLE,
      O => S(4)
    );
  S_4_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_4_ENABLE
    );
  A_12_IBUF_35 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(12),
      O => A_12_INBUF
    );
  A_12_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_12_INBUF,
      O => A_12_IBUF
    );
  S_5_OBUF_36 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_5_O,
      CTL => S_5_ENABLE,
      O => S(5)
    );
  S_5_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_5_ENABLE
    );
  A_13_IBUF_37 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(13),
      O => A_13_INBUF
    );
  A_13_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_13_INBUF,
      O => A_13_IBUF
    );
  S_6_OBUF_38 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_6_O,
      CTL => S_6_ENABLE,
      O => S(6)
    );
  S_6_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_6_ENABLE
    );
  A_14_IBUF_39 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(14),
      O => A_14_INBUF
    );
  A_14_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_14_INBUF,
      O => A_14_IBUF
    );
  S_7_OBUF_40 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_7_O,
      CTL => S_7_ENABLE,
      O => S(7)
    );
  S_7_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_7_ENABLE
    );
  A_15_IBUF_41 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A(15),
      O => A_15_INBUF
    );
  A_15_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => A_15_INBUF,
      O => A_15_IBUF
    );
  S_8_OBUF_42 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_8_O,
      CTL => S_8_ENABLE,
      O => S(8)
    );
  S_8_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_8_ENABLE
    );
  S_9_OBUF_43 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_9_O,
      CTL => S_9_ENABLE,
      O => S(9)
    );
  S_9_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => S_9_ENABLE
    );
  B_10_IBUF_44 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(10),
      O => B_10_INBUF
    );
  S_15_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXI_3_XLXN_5,
      O => XLXI_5_XLXI_3_XLXN_5_0
    );
  XLXI_5_XLXI_3_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"3C3C"
    )
    port map (
      ADR0 => VCC,
      ADR1 => B_14_IBUF,
      ADR2 => A_14_IBUF,
      ADR3 => VCC,
      O => XLXI_5_XLXI_3_XLXN_5
    );
  S_6_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_2_XLXN_5,
      O => XLXI_3_XLXI_2_XLXN_5_0
    );
  XLXI_3_XLXI_2_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"0FF0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => A_5_IBUF,
      ADR3 => B_5_IBUF,
      O => XLXI_3_XLXI_2_XLXN_5
    );
  XLXI_5_XLXI_1_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXI_1_XLXN_5,
      O => XLXI_5_XLXI_1_XLXN_5_0
    );
  XLXI_5_XLXI_2_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"366C"
    )
    port map (
      ADR0 => A_12_IBUF,
      ADR1 => XLXI_5_XLXI_2_XLXN_5_0,
      ADR2 => XLXN_3_0,
      ADR3 => B_12_IBUF,
      O => S_13_OBUF
    );
  XLXI_2_XLXI_4_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXI_4_XLXN_5,
      O => XLXI_2_XLXI_4_XLXN_5_0
    );
  XLXI_3_XLXI_1_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"17E8"
    )
    port map (
      ADR0 => A_3_IBUF,
      ADR1 => XLXI_2_XLXN_10,
      ADR2 => B_3_IBUF,
      ADR3 => XLXI_3_XLXI_1_XLXN_5_0,
      O => S_4_OBUF
    );
  S_11_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXI_3_XLXN_5,
      O => XLXI_4_XLXI_3_XLXN_5_0
    );
  XLXI_4_XLXI_3_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"0FF0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => B_10_IBUF,
      ADR3 => A_10_IBUF,
      O => XLXI_4_XLXI_3_XLXN_5
    );
  S_1_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXI_2_XLXN_5_pack_1,
      O => XLXI_2_XLXI_2_XLXN_5
    );
  XLXI_2_XLXI_2_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => A_1_IBUF,
      ADR2 => VCC,
      ADR3 => B_1_IBUF,
      O => XLXI_2_XLXI_2_XLXN_5_pack_1
    );
  XLXN_3_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_3,
      O => XLXN_3_0
    );
  XLXN_3_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXN_10_pack_1,
      O => XLXI_4_XLXN_10
    );
  XLXI_4_XLXI_3_XLXI_3 : X_LUT4
    generic map(
      INIT => X"E8E8"
    )
    port map (
      ADR0 => XLXI_4_XLXN_7_0,
      ADR1 => A_10_IBUF,
      ADR2 => B_10_IBUF,
      ADR3 => VCC,
      O => XLXI_4_XLXN_10_pack_1
    );
  XLXI_2_XLXI_1_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"9696"
    )
    port map (
      ADR0 => A_0_IBUF,
      ADR1 => B_0_IBUF,
      ADR2 => Ci_IBUF,
      ADR3 => VCC,
      O => S_0_OBUF
    );
  XLXI_2_XLXN_7_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_7,
      O => XLXI_2_XLXN_7_0
    );
  XLXI_2_XLXN_7_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_3_pack_1,
      O => XLXI_2_XLXN_3
    );
  XLXI_2_XLXI_1_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FCC0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => B_0_IBUF,
      ADR2 => A_0_IBUF,
      ADR3 => Ci_IBUF,
      O => XLXI_2_XLXN_3_pack_1
    );
  XLXI_5_XLXI_4_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXI_4_XLXN_5,
      O => XLXI_5_XLXI_4_XLXN_5_0
    );
  XLXI_5_XLXI_4_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"5A5A"
    )
    port map (
      ADR0 => A_15_IBUF,
      ADR1 => VCC,
      ADR2 => B_15_IBUF,
      ADR3 => VCC,
      O => XLXI_5_XLXI_4_XLXN_5
    );
  Co_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXN_10_pack_1,
      O => XLXI_5_XLXN_10
    );
  XLXI_5_XLXI_3_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FCC0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => A_14_IBUF,
      ADR2 => B_14_IBUF,
      ADR3 => XLXI_5_XLXN_7_0,
      O => XLXI_5_XLXN_10_pack_1
    );
  XLXI_3_XLXN_7_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXN_7,
      O => XLXI_3_XLXN_7_0
    );
  XLXI_3_XLXN_7_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXN_3_pack_1,
      O => XLXI_3_XLXN_3
    );
  XLXI_3_XLXI_1_XLXI_3 : X_LUT4
    generic map(
      INIT => X"E8E8"
    )
    port map (
      ADR0 => A_4_IBUF,
      ADR1 => B_4_IBUF,
      ADR2 => XLXN_1_0,
      ADR3 => VCC,
      O => XLXI_3_XLXN_3_pack_1
    );
  XLXI_2_XLXI_3_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"17E8"
    )
    port map (
      ADR0 => B_1_IBUF,
      ADR1 => A_1_IBUF,
      ADR2 => XLXI_2_XLXN_3,
      ADR3 => XLXI_2_XLXI_3_XLXN_5_0,
      O => S_2_OBUF
    );
  B_10_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_10_INBUF,
      O => B_10_IBUF
    );
  B_11_IBUF_45 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(11),
      O => B_11_INBUF
    );
  B_11_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_11_INBUF,
      O => B_11_IBUF
    );
  B_12_IBUF_46 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(12),
      O => B_12_INBUF
    );
  B_12_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_12_INBUF,
      O => B_12_IBUF
    );
  B_13_IBUF_47 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(13),
      O => B_13_INBUF
    );
  B_13_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_13_INBUF,
      O => B_13_IBUF
    );
  B_14_IBUF_48 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(14),
      O => B_14_INBUF
    );
  B_14_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_14_INBUF,
      O => B_14_IBUF
    );
  B_15_IBUF_49 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B(15),
      O => B_15_INBUF
    );
  B_15_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => B_15_INBUF,
      O => B_15_IBUF
    );
  XLXI_4_XLXN_7_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXN_7,
      O => XLXI_4_XLXN_7_0
    );
  XLXI_4_XLXN_7_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXN_3_pack_1,
      O => XLXI_4_XLXN_3
    );
  XLXI_4_XLXI_1_XLXI_3 : X_LUT4
    generic map(
      INIT => X"E8E8"
    )
    port map (
      ADR0 => B_8_IBUF,
      ADR1 => XLXN_2_0,
      ADR2 => A_8_IBUF,
      ADR3 => VCC,
      O => XLXI_4_XLXN_3_pack_1
    );
  S_9_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXI_1_XLXN_5,
      O => XLXI_4_XLXI_1_XLXN_5_0
    );
  XLXI_4_XLXI_1_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => A_8_IBUF,
      ADR2 => VCC,
      ADR3 => B_8_IBUF,
      O => XLXI_4_XLXI_1_XLXN_5
    );
  XLXI_3_XLXI_3_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_3_XLXN_5,
      O => XLXI_3_XLXI_3_XLXN_5_0
    );
  XLXI_3_XLXI_4_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"17E8"
    )
    port map (
      ADR0 => XLXI_3_XLXN_7_0,
      ADR1 => A_6_IBUF,
      ADR2 => B_6_IBUF,
      ADR3 => XLXI_3_XLXI_4_XLXN_5_0,
      O => S_7_OBUF
    );
  XLXN_1_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_1,
      O => XLXN_1_0
    );
  XLXN_1_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_10_pack_1,
      O => XLXI_2_XLXN_10
    );
  XLXI_2_XLXI_3_XLXI_3 : X_LUT4
    generic map(
      INIT => X"EE88"
    )
    port map (
      ADR0 => XLXI_2_XLXN_7_0,
      ADR1 => A_2_IBUF,
      ADR2 => VCC,
      ADR3 => B_2_IBUF,
      O => XLXI_2_XLXN_10_pack_1
    );
  S_14_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXI_2_XLXN_5,
      O => XLXI_5_XLXI_2_XLXN_5_0
    );
  XLXI_5_XLXI_2_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"5A5A"
    )
    port map (
      ADR0 => B_13_IBUF,
      ADR1 => VCC,
      ADR2 => A_13_IBUF,
      ADR3 => VCC,
      O => XLXI_5_XLXI_2_XLXN_5
    );
  S_5_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_1_XLXN_5,
      O => XLXI_3_XLXI_1_XLXN_5_0
    );
  XLXI_3_XLXI_1_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"0FF0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => B_4_IBUF,
      ADR3 => A_4_IBUF,
      O => XLXI_3_XLXI_1_XLXN_5
    );
  XLXI_4_XLXI_4_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXI_4_XLXN_5,
      O => XLXI_4_XLXI_4_XLXN_5_0
    );
  XLXI_5_XLXI_1_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"366C"
    )
    port map (
      ADR0 => B_11_IBUF,
      ADR1 => XLXI_5_XLXI_1_XLXN_5_0,
      ADR2 => XLXI_4_XLXN_10,
      ADR3 => A_11_IBUF,
      O => S_12_OBUF
    );
  XLXI_2_XLXI_3_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXI_3_XLXN_5,
      O => XLXI_2_XLXI_3_XLXN_5_0
    );
  XLXI_2_XLXI_4_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"366C"
    )
    port map (
      ADR0 => B_2_IBUF,
      ADR1 => XLXI_2_XLXI_4_XLXN_5_0,
      ADR2 => A_2_IBUF,
      ADR3 => XLXI_2_XLXN_7_0,
      O => S_3_OBUF
    );
  XLXI_5_XLXN_7_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXN_7,
      O => XLXI_5_XLXN_7_0
    );
  XLXI_5_XLXN_7_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_XLXN_3_pack_1,
      O => XLXI_5_XLXN_3
    );
  XLXI_5_XLXI_1_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FCC0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => B_12_IBUF,
      ADR2 => XLXN_3_0,
      ADR3 => A_12_IBUF,
      O => XLXI_5_XLXN_3_pack_1
    );
  S_10_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_XLXI_2_XLXN_5,
      O => XLXI_4_XLXI_2_XLXN_5_0
    );
  XLXI_4_XLXI_2_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"0FF0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => A_9_IBUF,
      ADR3 => B_9_IBUF,
      O => XLXI_4_XLXI_2_XLXN_5
    );
  XLXN_2_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_2,
      O => XLXN_2_0
    );
  XLXN_2_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXN_10_pack_1,
      O => XLXI_3_XLXN_10
    );
  XLXI_3_XLXI_3_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FCC0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => B_6_IBUF,
      ADR2 => A_6_IBUF,
      ADR3 => XLXI_3_XLXN_7_0,
      O => XLXI_3_XLXN_10_pack_1
    );
  XLXI_4_XLXI_1_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"1E78"
    )
    port map (
      ADR0 => B_7_IBUF,
      ADR1 => XLXI_3_XLXN_10,
      ADR2 => XLXI_4_XLXI_1_XLXN_5_0,
      ADR3 => A_7_IBUF,
      O => S_8_OBUF
    );
  XLXI_3_XLXI_4_XLXN_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_4_XLXN_5,
      O => XLXI_3_XLXI_4_XLXN_5_0
    );
  XLXI_2_XLXI_3_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"5A5A"
    )
    port map (
      ADR0 => B_2_IBUF,
      ADR1 => VCC,
      ADR2 => A_2_IBUF,
      ADR3 => VCC,
      O => XLXI_2_XLXI_3_XLXN_5
    );
  XLXI_5_XLXI_2_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FCC0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => A_13_IBUF,
      ADR2 => XLXI_5_XLXN_3,
      ADR3 => B_13_IBUF,
      O => XLXI_5_XLXN_7
    );
  XLXI_4_XLXI_3_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"566A"
    )
    port map (
      ADR0 => XLXI_4_XLXI_3_XLXN_5_0,
      ADR1 => XLXI_4_XLXN_3,
      ADR2 => A_9_IBUF,
      ADR3 => B_9_IBUF,
      O => S_10_OBUF
    );
  XLXI_3_XLXI_4_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => A_7_IBUF,
      ADR1 => VCC,
      ADR2 => XLXI_3_XLXN_10,
      ADR3 => B_7_IBUF,
      O => XLXN_2
    );
  XLXI_3_XLXI_4_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => B_7_IBUF,
      ADR2 => VCC,
      ADR3 => A_7_IBUF,
      O => XLXI_3_XLXI_4_XLXN_5
    );
  XLXI_5_XLXI_4_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"1E78"
    )
    port map (
      ADR0 => XLXI_5_XLXN_7_0,
      ADR1 => B_14_IBUF,
      ADR2 => XLXI_5_XLXI_4_XLXN_5_0,
      ADR3 => A_14_IBUF,
      O => S_15_OBUF
    );
  XLXI_3_XLXI_3_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"366C"
    )
    port map (
      ADR0 => XLXI_3_XLXN_3,
      ADR1 => XLXI_3_XLXI_3_XLXN_5_0,
      ADR2 => A_5_IBUF,
      ADR3 => B_5_IBUF,
      O => S_6_OBUF
    );
  XLXI_5_XLXI_1_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => B_12_IBUF,
      ADR2 => VCC,
      ADR3 => A_12_IBUF,
      O => XLXI_5_XLXI_1_XLXN_5
    );
  XLXI_2_XLXI_4_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"5A5A"
    )
    port map (
      ADR0 => A_3_IBUF,
      ADR1 => VCC,
      ADR2 => B_3_IBUF,
      ADR3 => VCC,
      O => XLXI_2_XLXI_4_XLXN_5
    );
  XLXI_4_XLXI_2_XLXI_3 : X_LUT4
    generic map(
      INIT => X"E8E8"
    )
    port map (
      ADR0 => B_9_IBUF,
      ADR1 => A_9_IBUF,
      ADR2 => XLXI_4_XLXN_3,
      ADR3 => VCC,
      O => XLXI_4_XLXN_7
    );
  XLXI_4_XLXI_2_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"566A"
    )
    port map (
      ADR0 => XLXI_4_XLXI_2_XLXN_5_0,
      ADR1 => A_8_IBUF,
      ADR2 => XLXN_2_0,
      ADR3 => B_8_IBUF,
      O => S_9_OBUF
    );
  XLXI_3_XLXI_3_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"3C3C"
    )
    port map (
      ADR0 => VCC,
      ADR1 => A_6_IBUF,
      ADR2 => B_6_IBUF,
      ADR3 => VCC,
      O => XLXI_3_XLXI_3_XLXN_5
    );
  XLXI_2_XLXI_4_XLXI_3 : X_LUT4
    generic map(
      INIT => X"EE88"
    )
    port map (
      ADR0 => A_3_IBUF,
      ADR1 => B_3_IBUF,
      ADR2 => VCC,
      ADR3 => XLXI_2_XLXN_10,
      O => XLXN_1
    );
  XLXI_5_XLXI_3_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"17E8"
    )
    port map (
      ADR0 => B_13_IBUF,
      ADR1 => XLXI_5_XLXN_3,
      ADR2 => A_13_IBUF,
      ADR3 => XLXI_5_XLXI_3_XLXN_5_0,
      O => S_14_OBUF
    );
  XLXI_3_XLXI_2_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"366C"
    )
    port map (
      ADR0 => XLXN_1_0,
      ADR1 => XLXI_3_XLXI_2_XLXN_5_0,
      ADR2 => B_4_IBUF,
      ADR3 => A_4_IBUF,
      O => S_5_OBUF
    );
  XLXI_4_XLXI_4_XLXI_1_XLXI_1 : X_LUT4
    generic map(
      INIT => X"55AA"
    )
    port map (
      ADR0 => B_11_IBUF,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => A_11_IBUF,
      O => XLXI_4_XLXI_4_XLXN_5
    );
  XLXI_4_XLXI_4_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"566A"
    )
    port map (
      ADR0 => XLXI_4_XLXI_4_XLXN_5_0,
      ADR1 => XLXI_4_XLXN_7_0,
      ADR2 => B_10_IBUF,
      ADR3 => A_10_IBUF,
      O => S_11_OBUF
    );
  XLXI_2_XLXI_2_XLXI_2_XLXI_1 : X_LUT4
    generic map(
      INIT => X"17E8"
    )
    port map (
      ADR0 => Ci_IBUF,
      ADR1 => A_0_IBUF,
      ADR2 => B_0_IBUF,
      ADR3 => XLXI_2_XLXI_2_XLXN_5,
      O => S_1_OBUF
    );
  XLXI_4_XLXI_4_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => B_11_IBUF,
      ADR1 => VCC,
      ADR2 => XLXI_4_XLXN_10,
      ADR3 => A_11_IBUF,
      O => XLXN_3
    );
  XLXI_2_XLXI_2_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => B_1_IBUF,
      ADR1 => VCC,
      ADR2 => A_1_IBUF,
      ADR3 => XLXI_2_XLXN_3,
      O => XLXI_2_XLXN_7
    );
  XLXI_5_XLXI_4_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => A_15_IBUF,
      ADR1 => VCC,
      ADR2 => B_15_IBUF,
      ADR3 => XLXI_5_XLXN_10,
      O => Co_OBUF
    );
  XLXI_3_XLXI_2_XLXI_3 : X_LUT4
    generic map(
      INIT => X"FAA0"
    )
    port map (
      ADR0 => XLXI_3_XLXN_3,
      ADR1 => VCC,
      ADR2 => A_5_IBUF,
      ADR3 => B_5_IBUF,
      O => XLXI_3_XLXN_7
    );
  S_10_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_10_OBUF,
      O => S_10_O
    );
  S_11_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_11_OBUF,
      O => S_11_O
    );
  Co_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => Co_OBUF,
      O => Co_O
    );
  S_12_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_12_OBUF,
      O => S_12_O
    );
  S_13_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_13_OBUF,
      O => S_13_O
    );
  S_14_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_14_OBUF,
      O => S_14_O
    );
  S_15_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_15_OBUF,
      O => S_15_O
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
  S_4_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_4_OBUF,
      O => S_4_O
    );
  S_5_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_5_OBUF,
      O => S_5_O
    );
  S_6_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_6_OBUF,
      O => S_6_O
    );
  S_7_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_7_OBUF,
      O => S_7_O
    );
  S_8_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_8_OBUF,
      O => S_8_O
    );
  S_9_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => S_9_OBUF,
      O => S_9_O
    );
  NlwBlock_sixteenbit_adder_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

