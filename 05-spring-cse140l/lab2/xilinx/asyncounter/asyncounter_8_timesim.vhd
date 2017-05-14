--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: asyncounter_8_timesim.vhd
-- /___/   /\     Timestamp: Wed May 04 00:28:08 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf asyncounter_8.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim asyncounter_8.ncd asyncounter_8_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: asyncounter_8.ncd
-- Output file	: asyncounter_8_timesim.vhd
-- # of Entities	: 1
-- Design Name	: asyncounter_8
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

entity asyncounter_8 is
  port (
    CLK : in STD_LOGIC := 'X'; 
    CLR : in STD_LOGIC := 'X'; 
    VCC : in STD_LOGIC := 'X'; 
    CLK_CE : in STD_LOGIC := 'X'; 
    D : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end asyncounter_8;

architecture Structure of asyncounter_8 is
  signal NlwRenamedSignal_VCC : STD_LOGIC; 
  signal CLK_CE_IBUF : STD_LOGIC; 
  signal CLK_BUFGP : STD_LOGIC; 
  signal CLR_IBUF : STD_LOGIC; 
  signal VCC_IBUF : STD_LOGIC; 
  signal D_0_OBUF : STD_LOGIC; 
  signal D_1_OBUF : STD_LOGIC; 
  signal D_2_OBUF : STD_LOGIC; 
  signal D_3_OBUF : STD_LOGIC; 
  signal D_4_OBUF : STD_LOGIC; 
  signal D_5_OBUF : STD_LOGIC; 
  signal D_6_OBUF : STD_LOGIC; 
  signal D_7_OBUF : STD_LOGIC; 
  signal CLK_BUFGP_IBUFG : STD_LOGIC; 
  signal GLOBAL_LOGIC1 : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal D_0_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_2_TQ_pack_1 : STD_LOGIC; 
  signal D_0_OBUF_CLKINV : STD_LOGIC; 
  signal D_0_OBUF_CEINV : STD_LOGIC; 
  signal D_2_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_4_TQ_pack_1 : STD_LOGIC; 
  signal D_2_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_2_OBUF_CEINV : STD_LOGIC; 
  signal D_4_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_10_TQ_pack_1 : STD_LOGIC; 
  signal D_4_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_4_OBUF_CEINV : STD_LOGIC; 
  signal D_5_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_12_TQ_pack_1 : STD_LOGIC; 
  signal D_5_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_5_OBUF_CEINV : STD_LOGIC; 
  signal D_1_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_3_TQ_pack_1 : STD_LOGIC; 
  signal D_1_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_1_OBUF_CEINV : STD_LOGIC; 
  signal D_3_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_5_TQ_pack_1 : STD_LOGIC; 
  signal D_3_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_3_OBUF_CEINV : STD_LOGIC; 
  signal D_7_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_11_TQ_pack_1 : STD_LOGIC; 
  signal D_7_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_7_OBUF_CEINV : STD_LOGIC; 
  signal D_6_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_13_TQ_pack_1 : STD_LOGIC; 
  signal D_6_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D_6_OBUF_CEINV : STD_LOGIC; 
  signal D_0_ENABLE : STD_LOGIC; 
  signal D_0_O : STD_LOGIC; 
  signal D_1_ENABLE : STD_LOGIC; 
  signal D_1_O : STD_LOGIC; 
  signal D_2_ENABLE : STD_LOGIC; 
  signal D_2_O : STD_LOGIC; 
  signal D_3_ENABLE : STD_LOGIC; 
  signal D_3_O : STD_LOGIC; 
  signal VCC_INBUF : STD_LOGIC; 
  signal D_4_ENABLE : STD_LOGIC; 
  signal D_4_O : STD_LOGIC; 
  signal D_5_ENABLE : STD_LOGIC; 
  signal D_5_O : STD_LOGIC; 
  signal D_6_ENABLE : STD_LOGIC; 
  signal D_6_O : STD_LOGIC; 
  signal D_7_ENABLE : STD_LOGIC; 
  signal D_7_O : STD_LOGIC; 
  signal CLK_INBUF : STD_LOGIC; 
  signal CLR_INBUF : STD_LOGIC; 
  signal CLK_CE_INBUF : STD_LOGIC; 
  signal CLK_BUFGP_BUFG_S_INVNOT : STD_LOGIC; 
  signal D_0_OBUF_FFY_RST : STD_LOGIC; 
  signal D_0_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_2_OBUF_FFY_RST : STD_LOGIC; 
  signal D_2_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_4_OBUF_FFY_RST : STD_LOGIC; 
  signal D_4_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_5_OBUF_FFY_RST : STD_LOGIC; 
  signal D_5_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_1_OBUF_FFY_RST : STD_LOGIC; 
  signal D_1_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_3_OBUF_FFY_RST : STD_LOGIC; 
  signal D_3_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_7_OBUF_FFY_RST : STD_LOGIC; 
  signal D_7_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_6_OBUF_FFY_RST : STD_LOGIC; 
  signal D_6_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal VCC_0 : STD_LOGIC; 
begin
  NlwRenamedSignal_VCC <= VCC;
  D_0_OBUF_DYMUX_1 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_TQ_pack_1,
      O => D_0_OBUF_DYMUX
    );
  D_0_OBUF_CLKINV_2 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_0_OBUF_CLKINV
    );
  D_0_OBUF_CEINV_3 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_0_OBUF_CEINV
    );
  D_2_OBUF_DYMUX_4 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_TQ_pack_1,
      O => D_2_OBUF_DYMUX
    );
  D_2_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_1_OBUF,
      O => D_2_OBUF_CLKINVNOT
    );
  D_2_OBUF_CEINV_5 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_2_OBUF_CEINV
    );
  D_4_OBUF_DYMUX_6 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_10_TQ_pack_1,
      O => D_4_OBUF_DYMUX
    );
  D_4_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_3_OBUF,
      O => D_4_OBUF_CLKINVNOT
    );
  D_4_OBUF_CEINV_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_4_OBUF_CEINV
    );
  D_5_OBUF_DYMUX_8 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_12_TQ_pack_1,
      O => D_5_OBUF_DYMUX
    );
  D_5_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_4_OBUF,
      O => D_5_OBUF_CLKINVNOT
    );
  D_5_OBUF_CEINV_9 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_5_OBUF_CEINV
    );
  D_1_OBUF_DYMUX_10 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_TQ_pack_1,
      O => D_1_OBUF_DYMUX
    );
  D_1_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_0_OBUF,
      O => D_1_OBUF_CLKINVNOT
    );
  D_1_OBUF_CEINV_11 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_1_OBUF_CEINV
    );
  D_3_OBUF_DYMUX_12 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_TQ_pack_1,
      O => D_3_OBUF_DYMUX
    );
  D_3_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_2_OBUF,
      O => D_3_OBUF_CLKINVNOT
    );
  D_3_OBUF_CEINV_13 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_3_OBUF_CEINV
    );
  D_7_OBUF_DYMUX_14 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_11_TQ_pack_1,
      O => D_7_OBUF_DYMUX
    );
  D_7_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_6_OBUF,
      O => D_7_OBUF_CLKINVNOT
    );
  D_7_OBUF_CEINV_15 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_7_OBUF_CEINV
    );
  D_6_OBUF_DYMUX_16 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_13_TQ_pack_1,
      O => D_6_OBUF_DYMUX
    );
  D_6_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_5_OBUF,
      O => D_6_OBUF_CLKINVNOT
    );
  D_6_OBUF_CEINV_17 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_6_OBUF_CEINV
    );
  D_0_OBUF_18 : X_TRI_PP
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
  D_1_OBUF_19 : X_TRI_PP
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
  D_2_OBUF_20 : X_TRI_PP
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
  D_3_OBUF_21 : X_TRI_PP
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
  VCC_IBUF_22 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => NlwRenamedSignal_VCC,
      O => VCC_INBUF
    );
  VCC_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => VCC_INBUF,
      O => VCC_IBUF
    );
  D_4_OBUF_23 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_4_O,
      CTL => D_4_ENABLE,
      O => D(4)
    );
  D_4_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_4_ENABLE
    );
  D_5_OBUF_24 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_5_O,
      CTL => D_5_ENABLE,
      O => D(5)
    );
  D_5_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_5_ENABLE
    );
  D_6_OBUF_25 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_6_O,
      CTL => D_6_ENABLE,
      O => D(6)
    );
  D_6_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_6_ENABLE
    );
  D_7_OBUF_26 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_7_O,
      CTL => D_7_ENABLE,
      O => D(7)
    );
  D_7_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D_7_ENABLE
    );
  CLK_BUFGP_IBUFG_27 : X_BUF_PP
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
  CLR_IBUF_28 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR,
      O => CLR_INBUF
    );
  CLR_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_INBUF,
      O => CLR_IBUF
    );
  CLK_CE_IBUF_29 : X_BUF_PP
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
  XLXI_2_I_36_32 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => VCC_IBUF,
      ADR2 => VCC_0,
      ADR3 => D_0_OBUF,
      O => XLXI_2_TQ_pack_1
    );
  XLXI_2_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_0_OBUF_DYMUX,
      CE => D_0_OBUF_CEINV,
      CLK => D_0_OBUF_CLKINV,
      SET => GND,
      RST => D_0_OBUF_FFY_RST,
      O => D_0_OBUF
    );
  D_0_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_0_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_0_OBUF_FFY_RST
    );
  D_0_OBUF_FFY_RSTAND_30 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_0_OBUF_FFY_RSTAND
    );
  XLXI_4_I_36_32 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => VCC_IBUF,
      ADR2 => VCC_0,
      ADR3 => D_2_OBUF,
      O => XLXI_4_TQ_pack_1
    );
  XLXI_4_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_2_OBUF_DYMUX,
      CE => D_2_OBUF_CEINV,
      CLK => D_2_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_2_OBUF_FFY_RST,
      O => D_2_OBUF
    );
  D_2_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_2_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_2_OBUF_FFY_RST
    );
  D_2_OBUF_FFY_RSTAND_31 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_2_OBUF_FFY_RSTAND
    );
  XLXI_10_I_36_32 : X_LUT4
    generic map(
      INIT => X"55AA"
    )
    port map (
      ADR0 => VCC_IBUF,
      ADR1 => VCC_0,
      ADR2 => VCC_0,
      ADR3 => D_4_OBUF,
      O => XLXI_10_TQ_pack_1
    );
  XLXI_10_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_4_OBUF_DYMUX,
      CE => D_4_OBUF_CEINV,
      CLK => D_4_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_4_OBUF_FFY_RST,
      O => D_4_OBUF
    );
  D_4_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_4_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_4_OBUF_FFY_RST
    );
  D_4_OBUF_FFY_RSTAND_32 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_4_OBUF_FFY_RSTAND
    );
  XLXI_12_I_36_32 : X_LUT4
    generic map(
      INIT => X"3C3C"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => D_5_OBUF,
      ADR2 => VCC_IBUF,
      ADR3 => VCC_0,
      O => XLXI_12_TQ_pack_1
    );
  XLXI_12_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_5_OBUF_DYMUX,
      CE => D_5_OBUF_CEINV,
      CLK => D_5_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_5_OBUF_FFY_RST,
      O => D_5_OBUF
    );
  D_5_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_5_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_5_OBUF_FFY_RST
    );
  D_5_OBUF_FFY_RSTAND_33 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_5_OBUF_FFY_RSTAND
    );
  XLXI_3_I_36_32 : X_LUT4
    generic map(
      INIT => X"5A5A"
    )
    port map (
      ADR0 => D_1_OBUF,
      ADR1 => VCC_0,
      ADR2 => VCC_IBUF,
      ADR3 => VCC_0,
      O => XLXI_3_TQ_pack_1
    );
  XLXI_3_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_1_OBUF_DYMUX,
      CE => D_1_OBUF_CEINV,
      CLK => D_1_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_1_OBUF_FFY_RST,
      O => D_1_OBUF
    );
  D_1_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_1_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_1_OBUF_FFY_RST
    );
  D_1_OBUF_FFY_RSTAND_34 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_1_OBUF_FFY_RSTAND
    );
  XLXI_5_I_36_32 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => D_3_OBUF,
      ADR2 => VCC_0,
      ADR3 => VCC_IBUF,
      O => XLXI_5_TQ_pack_1
    );
  XLXI_5_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_3_OBUF_DYMUX,
      CE => D_3_OBUF_CEINV,
      CLK => D_3_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_3_OBUF_FFY_RST,
      O => D_3_OBUF
    );
  D_3_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_3_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_3_OBUF_FFY_RST
    );
  D_3_OBUF_FFY_RSTAND_35 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_3_OBUF_FFY_RSTAND
    );
  XLXI_11_I_36_32 : X_LUT4
    generic map(
      INIT => X"55AA"
    )
    port map (
      ADR0 => VCC_IBUF,
      ADR1 => VCC_0,
      ADR2 => VCC_0,
      ADR3 => D_7_OBUF,
      O => XLXI_11_TQ_pack_1
    );
  XLXI_11_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_7_OBUF_DYMUX,
      CE => D_7_OBUF_CEINV,
      CLK => D_7_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_7_OBUF_FFY_RST,
      O => D_7_OBUF
    );
  D_7_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_7_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_7_OBUF_FFY_RST
    );
  D_7_OBUF_FFY_RSTAND_36 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_7_OBUF_FFY_RSTAND
    );
  XLXI_13_I_36_32 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => D_6_OBUF,
      ADR2 => VCC_0,
      ADR3 => VCC_IBUF,
      O => XLXI_13_TQ_pack_1
    );
  XLXI_13_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_6_OBUF_DYMUX,
      CE => D_6_OBUF_CEINV,
      CLK => D_6_OBUF_CLKINVNOT,
      SET => GND,
      RST => D_6_OBUF_FFY_RST,
      O => D_6_OBUF
    );
  D_6_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D_6_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D_6_OBUF_FFY_RST
    );
  D_6_OBUF_FFY_RSTAND_37 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_6_OBUF_FFY_RSTAND
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
  D_4_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_4_OBUF,
      O => D_4_O
    );
  D_5_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_5_OBUF,
      O => D_5_O
    );
  D_6_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_6_OBUF,
      O => D_6_O
    );
  D_7_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D_7_OBUF,
      O => D_7_O
    );
  NlwBlock_asyncounter_8_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_asyncounter_8_VCC : X_ONE
    port map (
      O => VCC_0
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

