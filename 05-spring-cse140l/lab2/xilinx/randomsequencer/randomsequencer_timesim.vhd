--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: randomsequencer_timesim.vhd
-- /___/   /\     Timestamp: Tue May 03 23:16:05 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf randomsequencer.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim randomsequencer.ncd randomsequencer_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: randomsequencer.ncd
-- Output file	: randomsequencer_timesim.vhd
-- # of Entities	: 1
-- Design Name	: randomsequencer
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

entity randomsequencer is
  port (
    CLK : in STD_LOGIC := 'X'; 
    PRE : in STD_LOGIC := 'X'; 
    CLK_CE : in STD_LOGIC := 'X'; 
    D : out STD_LOGIC_VECTOR ( 3 downto 0 ) 
  );
end randomsequencer;

architecture Structure of randomsequencer is
  signal D_0_OBUF : STD_LOGIC; 
  signal D_1_OBUF : STD_LOGIC; 
  signal D_2_OBUF : STD_LOGIC; 
  signal D_3_OBUF : STD_LOGIC; 
  signal PRE_IBUF : STD_LOGIC; 
  signal CLK_BUFGP_IBUFG : STD_LOGIC; 
  signal CLK_CE_IBUF : STD_LOGIC; 
  signal GLOBAL_LOGIC1 : STD_LOGIC; 
  signal CLK_BUFGP : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal D_0_ENABLE : STD_LOGIC; 
  signal D_0_O : STD_LOGIC; 
  signal D_1_ENABLE : STD_LOGIC; 
  signal D_1_O : STD_LOGIC; 
  signal D_2_ENABLE : STD_LOGIC; 
  signal D_2_O : STD_LOGIC; 
  signal D_3_ENABLE : STD_LOGIC; 
  signal D_3_O : STD_LOGIC; 
  signal PRE_INBUF : STD_LOGIC; 
  signal CLK_INBUF : STD_LOGIC; 
  signal CLK_CE_INBUF : STD_LOGIC; 
  signal CLK_BUFGP_BUFG_S_INVNOT : STD_LOGIC; 
  signal D_0_OBUF_DYMUX : STD_LOGIC; 
  signal XLXN_8_pack_1 : STD_LOGIC; 
  signal D_0_OBUF_SRFFMUX : STD_LOGIC; 
  signal D_0_OBUF_CLKINV : STD_LOGIC; 
  signal D_0_OBUF_CEINV : STD_LOGIC; 
  signal D_1_OBUF_DYMUX : STD_LOGIC; 
  signal D_1_OBUF_SRFFMUX : STD_LOGIC; 
  signal D_1_OBUF_CLKINV : STD_LOGIC; 
  signal D_1_OBUF_CEINV : STD_LOGIC; 
  signal D_2_OBUF_DYMUX : STD_LOGIC; 
  signal D_2_OBUF_SRFFMUX : STD_LOGIC; 
  signal D_2_OBUF_CLKINV : STD_LOGIC; 
  signal D_2_OBUF_CEINV : STD_LOGIC; 
  signal D_3_OBUF_DYMUX : STD_LOGIC; 
  signal D_3_OBUF_SRFFMUX : STD_LOGIC; 
  signal D_3_OBUF_CLKINV : STD_LOGIC; 
  signal D_3_OBUF_CEINV : STD_LOGIC; 
  signal D_2_OBUF_FFY_SET : STD_LOGIC; 
  signal D_3_OBUF_FFY_SET : STD_LOGIC; 
  signal D_0_OBUF_FFY_SET : STD_LOGIC; 
  signal D_1_OBUF_FFY_SET : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
begin
  D_0_OBUF_0 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_0_O,
      CTL => D_0_ENABLE,
      O => D(0)
    );
  D_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_0_ENABLE
    );
  D_1_OBUF_1 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_1_O,
      CTL => D_1_ENABLE,
      O => D(1)
    );
  D_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_1_ENABLE
    );
  D_2_OBUF_2 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_2_O,
      CTL => D_2_ENABLE,
      O => D(2)
    );
  D_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_2_ENABLE
    );
  D_3_OBUF_3 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_3_O,
      CTL => D_3_ENABLE,
      O => D(3)
    );
  D_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_3_ENABLE
    );
  PRE_IBUF_4 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => PRE,
      O => PRE_INBUF
    );
  PRE_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => PRE_INBUF,
      O => PRE_IBUF
    );
  CLK_BUFGP_IBUFG_5 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK,
      O => CLK_INBUF
    );
  CLK_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_INBUF,
      O => CLK_BUFGP_IBUFG
    );
  CLK_CE_IBUF_6 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE,
      O => CLK_CE_INBUF
    );
  CLK_CE_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_INBUF,
      O => CLK_CE_IBUF
    );
  CLK_BUFGP_BUFG : X_BUFGMUX
    port map (
      I0 => CLK_BUFGP_IBUFG,
      I1 => GND,
      S => CLK_BUFGP_BUFG_S_INVNOT,
      O => CLK_BUFGP,
      GSR => GSR
    );
  CLK_BUFGP_BUFG_SINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GLOBAL_LOGIC1,
      O => CLK_BUFGP_BUFG_S_INVNOT
    );
  D_0_OBUF_DYMUX_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_8_pack_1,
      O => D_0_OBUF_DYMUX
    );
  D_0_OBUF_SRFFMUX_8 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => PRE_IBUF,
      O => D_0_OBUF_SRFFMUX
    );
  D_0_OBUF_CLKINV_9 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_0_OBUF_CLKINV
    );
  D_0_OBUF_CEINV_10 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_0_OBUF_CEINV
    );
  D_1_OBUF_DYMUX_11 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_0_OBUF,
      O => D_1_OBUF_DYMUX
    );
  D_1_OBUF_SRFFMUX_12 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => PRE_IBUF,
      O => D_1_OBUF_SRFFMUX
    );
  D_1_OBUF_CLKINV_13 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_1_OBUF_CLKINV
    );
  D_1_OBUF_CEINV_14 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_1_OBUF_CEINV
    );
  D_2_OBUF_DYMUX_15 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_1_OBUF,
      O => D_2_OBUF_DYMUX
    );
  D_2_OBUF_SRFFMUX_16 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => PRE_IBUF,
      O => D_2_OBUF_SRFFMUX
    );
  D_2_OBUF_CLKINV_17 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_2_OBUF_CLKINV
    );
  D_2_OBUF_CEINV_18 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_2_OBUF_CEINV
    );
  D_3_OBUF_DYMUX_19 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_2_OBUF,
      O => D_3_OBUF_DYMUX
    );
  D_3_OBUF_SRFFMUX_20 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => PRE_IBUF,
      O => D_3_OBUF_SRFFMUX
    );
  D_3_OBUF_CLKINV_21 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_3_OBUF_CLKINV
    );
  D_3_OBUF_CEINV_22 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_3_OBUF_CEINV
    );
  XLXI_4 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => D_2_OBUF_DYMUX,
      CE => D_2_OBUF_CEINV,
      CLK => D_2_OBUF_CLKINV,
      SET => D_2_OBUF_FFY_SET,
      RST => GND,
      O => D_2_OBUF
    );
  D_2_OBUF_FFY_SETOR : X_OR2
    port map (
      I0 => GSR,
      I1 => D_2_OBUF_SRFFMUX,
      O => D_2_OBUF_FFY_SET
    );
  XLXI_5 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => D_3_OBUF_DYMUX,
      CE => D_3_OBUF_CEINV,
      CLK => D_3_OBUF_CLKINV,
      SET => D_3_OBUF_FFY_SET,
      RST => GND,
      O => D_3_OBUF
    );
  D_3_OBUF_FFY_SETOR : X_OR2
    port map (
      I0 => GSR,
      I1 => D_3_OBUF_SRFFMUX,
      O => D_3_OBUF_FFY_SET
    );
  XLXI_9 : X_LUT4
    generic map(
      INIT => X"55AA"
    )
    port map (
      ADR0 => D_2_OBUF,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => D_3_OBUF,
      O => XLXN_8_pack_1
    );
  XLXI_2 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => D_0_OBUF_DYMUX,
      CE => D_0_OBUF_CEINV,
      CLK => D_0_OBUF_CLKINV,
      SET => D_0_OBUF_FFY_SET,
      RST => GND,
      O => D_0_OBUF
    );
  D_0_OBUF_FFY_SETOR : X_OR2
    port map (
      I0 => GSR,
      I1 => D_0_OBUF_SRFFMUX,
      O => D_0_OBUF_FFY_SET
    );
  XLXI_3 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => D_1_OBUF_DYMUX,
      CE => D_1_OBUF_CEINV,
      CLK => D_1_OBUF_CLKINV,
      SET => D_1_OBUF_FFY_SET,
      RST => GND,
      O => D_1_OBUF
    );
  D_1_OBUF_FFY_SETOR : X_OR2
    port map (
      I0 => GSR,
      I1 => D_1_OBUF_SRFFMUX,
      O => D_1_OBUF_FFY_SET
    );
  GLOBAL_LOGIC1_VCC : X_ONE
    port map (
      O => GLOBAL_LOGIC1
    );
  D_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_0_OBUF,
      O => D_0_O
    );
  D_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_1_OBUF,
      O => D_1_O
    );
  D_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_2_OBUF,
      O => D_2_O
    );
  D_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_3_OBUF,
      O => D_3_O
    );
  NlwBlock_randomsequencer_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_randomsequencer_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

