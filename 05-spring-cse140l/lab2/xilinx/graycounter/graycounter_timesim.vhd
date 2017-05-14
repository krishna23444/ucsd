--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: graycounter_timesim.vhd
-- /___/   /\     Timestamp: Tue May 03 22:46:27 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf graycounter.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim graycounter.ncd graycounter_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: graycounter.ncd
-- Output file	: graycounter_timesim.vhd
-- # of Entities	: 1
-- Design Name	: graycounter
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

entity graycounter is
  port (
    CLK : in STD_LOGIC := 'X'; 
    CLR : in STD_LOGIC := 'X'; 
    CLK_CE : in STD_LOGIC := 'X'; 
    D : out STD_LOGIC_VECTOR ( 3 downto 0 ) 
  );
end graycounter;

architecture Structure of graycounter is
  signal CLK_CE_IBUF : STD_LOGIC; 
  signal CLK_BUFGP : STD_LOGIC; 
  signal CLR_IBUF : STD_LOGIC; 
  signal D_1_OBUF : STD_LOGIC; 
  signal D_0_OBUF : STD_LOGIC; 
  signal D_3_OBUF : STD_LOGIC; 
  signal D_2_OBUF : STD_LOGIC; 
  signal CLK_BUFGP_IBUFG : STD_LOGIC; 
  signal GLOBAL_LOGIC1 : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal D_1_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_2_TQ_pack_1 : STD_LOGIC; 
  signal D_1_OBUF_CLKINV : STD_LOGIC; 
  signal D_1_OBUF_CEINV : STD_LOGIC; 
  signal D_3_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_4_TQ_pack_1 : STD_LOGIC; 
  signal D_3_OBUF_CLKINV : STD_LOGIC; 
  signal D_3_OBUF_CEINV : STD_LOGIC; 
  signal D_0_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_1_TQ_pack_1 : STD_LOGIC; 
  signal D_0_OBUF_CLKINV : STD_LOGIC; 
  signal D_0_OBUF_CEINV : STD_LOGIC; 
  signal D_2_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_3_TQ_pack_1 : STD_LOGIC; 
  signal D_2_OBUF_CLKINV : STD_LOGIC; 
  signal D_2_OBUF_CEINV : STD_LOGIC; 
  signal D_0_ENABLE : STD_LOGIC; 
  signal D_0_O : STD_LOGIC; 
  signal D_1_ENABLE : STD_LOGIC; 
  signal D_1_O : STD_LOGIC; 
  signal D_2_ENABLE : STD_LOGIC; 
  signal D_2_O : STD_LOGIC; 
  signal D_3_ENABLE : STD_LOGIC; 
  signal D_3_O : STD_LOGIC; 
  signal CLK_INBUF : STD_LOGIC; 
  signal CLR_INBUF : STD_LOGIC; 
  signal CLK_CE_INBUF : STD_LOGIC; 
  signal CLK_BUFGP_BUFG_S_INVNOT : STD_LOGIC; 
  signal D_1_OBUF_FFY_RST : STD_LOGIC; 
  signal D_1_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_3_OBUF_FFY_RST : STD_LOGIC; 
  signal D_3_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_0_OBUF_FFY_RST : STD_LOGIC; 
  signal D_0_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D_2_OBUF_FFY_RST : STD_LOGIC; 
  signal D_2_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
begin
  D_1_OBUF_DYMUX_0 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_TQ_pack_1,
      O => D_1_OBUF_DYMUX
    );
  D_1_OBUF_CLKINV_1 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_1_OBUF_CLKINV
    );
  D_1_OBUF_CEINV_2 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_1_OBUF_CEINV
    );
  D_3_OBUF_DYMUX_3 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_TQ_pack_1,
      O => D_3_OBUF_DYMUX
    );
  D_3_OBUF_CLKINV_4 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_3_OBUF_CLKINV
    );
  D_3_OBUF_CEINV_5 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_3_OBUF_CEINV
    );
  D_0_OBUF_DYMUX_6 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_TQ_pack_1,
      O => D_0_OBUF_DYMUX
    );
  D_0_OBUF_CLKINV_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_0_OBUF_CLKINV
    );
  D_0_OBUF_CEINV_8 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_0_OBUF_CEINV
    );
  D_2_OBUF_DYMUX_9 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_TQ_pack_1,
      O => D_2_OBUF_DYMUX
    );
  D_2_OBUF_CLKINV_10 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D_2_OBUF_CLKINV
    );
  D_2_OBUF_CEINV_11 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D_2_OBUF_CEINV
    );
  D_0_OBUF_12 : X_TRI_PP
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
  D_1_OBUF_13 : X_TRI_PP
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
  D_2_OBUF_14 : X_TRI_PP
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
  D_3_OBUF_15 : X_TRI_PP
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
  CLK_BUFGP_IBUFG_16 : X_BUF_PP
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
  CLR_IBUF_17 : X_BUF_PP
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
  CLK_CE_IBUF_18 : X_BUF_PP
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
      INIT => X"C3AA"
    )
    port map (
      ADR0 => D_1_OBUF,
      ADR1 => D_2_OBUF,
      ADR2 => D_3_OBUF,
      ADR3 => D_0_OBUF,
      O => XLXI_2_TQ_pack_1
    );
  XLXI_2_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_1_OBUF_DYMUX,
      CE => D_1_OBUF_CEINV,
      CLK => D_1_OBUF_CLKINV,
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
  D_1_OBUF_FFY_RSTAND_19 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_1_OBUF_FFY_RSTAND
    );
  XLXI_4_I_36_32 : X_LUT4
    generic map(
      INIT => X"F0E4"
    )
    port map (
      ADR0 => D_1_OBUF,
      ADR1 => D_2_OBUF,
      ADR2 => D_3_OBUF,
      ADR3 => D_0_OBUF,
      O => XLXI_4_TQ_pack_1
    );
  XLXI_4_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_3_OBUF_DYMUX,
      CE => D_3_OBUF_CEINV,
      CLK => D_3_OBUF_CLKINV,
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
  D_3_OBUF_FFY_RSTAND_20 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_3_OBUF_FFY_RSTAND
    );
  XLXI_1_I_36_32 : X_LUT4
    generic map(
      INIT => X"3CC3"
    )
    port map (
      ADR0 => VCC,
      ADR1 => D_3_OBUF,
      ADR2 => D_2_OBUF,
      ADR3 => D_1_OBUF,
      O => XLXI_1_TQ_pack_1
    );
  XLXI_1_I_36_35 : X_FF
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
  D_0_OBUF_FFY_RSTAND_21 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_0_OBUF_FFY_RSTAND
    );
  XLXI_3_I_36_32 : X_LUT4
    generic map(
      INIT => X"B1F0"
    )
    port map (
      ADR0 => D_0_OBUF,
      ADR1 => D_3_OBUF,
      ADR2 => D_2_OBUF,
      ADR3 => D_1_OBUF,
      O => XLXI_3_TQ_pack_1
    );
  XLXI_3_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D_2_OBUF_DYMUX,
      CE => D_2_OBUF_CEINV,
      CLK => D_2_OBUF_CLKINV,
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
  D_2_OBUF_FFY_RSTAND_22 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D_2_OBUF_FFY_RSTAND
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
  NlwBlock_graycounter_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_graycounter_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

