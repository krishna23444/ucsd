--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: asyncounter_timesim.vhd
-- /___/   /\     Timestamp: Tue May 03 03:01:01 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf asyncounter.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim asyncounter.ncd asyncounter_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: asyncounter.ncd
-- Output file	: asyncounter_timesim.vhd
-- # of Entities	: 1
-- Design Name	: asyncounter
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

entity asyncounter is
  port (
    CLK : in STD_LOGIC := 'X'; 
    CLR : in STD_LOGIC := 'X'; 
    VCC : in STD_LOGIC := 'X'; 
    CLK_CE : in STD_LOGIC := 'X'; 
    D0 : out STD_LOGIC; 
    D1 : out STD_LOGIC; 
    D2 : out STD_LOGIC; 
    D3 : out STD_LOGIC 
  );
end asyncounter;

architecture Structure of asyncounter is
  signal NlwRenamedSignal_VCC : STD_LOGIC; 
  signal CLK_CE_IBUF : STD_LOGIC; 
  signal CLK_BUFGP : STD_LOGIC; 
  signal CLR_IBUF : STD_LOGIC; 
  signal VCC_IBUF : STD_LOGIC; 
  signal D0_OBUF : STD_LOGIC; 
  signal D1_OBUF : STD_LOGIC; 
  signal D2_OBUF : STD_LOGIC; 
  signal D3_OBUF : STD_LOGIC; 
  signal CLK_BUFGP_IBUFG : STD_LOGIC; 
  signal GLOBAL_LOGIC1 : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal D0_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_2_TQ_pack_1 : STD_LOGIC; 
  signal D0_OBUF_CLKINV : STD_LOGIC; 
  signal D0_OBUF_CEINV : STD_LOGIC; 
  signal D2_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_4_TQ_pack_1 : STD_LOGIC; 
  signal D2_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D2_OBUF_CEINV : STD_LOGIC; 
  signal D1_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_3_TQ_pack_1 : STD_LOGIC; 
  signal D1_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D1_OBUF_CEINV : STD_LOGIC; 
  signal D3_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_5_TQ_pack_1 : STD_LOGIC; 
  signal D3_OBUF_CLKINVNOT : STD_LOGIC; 
  signal D3_OBUF_CEINV : STD_LOGIC; 
  signal VCC_INBUF : STD_LOGIC; 
  signal D0_ENABLE : STD_LOGIC; 
  signal D0_O : STD_LOGIC; 
  signal D1_ENABLE : STD_LOGIC; 
  signal D1_O : STD_LOGIC; 
  signal D2_ENABLE : STD_LOGIC; 
  signal D2_O : STD_LOGIC; 
  signal D3_ENABLE : STD_LOGIC; 
  signal D3_O : STD_LOGIC; 
  signal CLK_INBUF : STD_LOGIC; 
  signal CLR_INBUF : STD_LOGIC; 
  signal CLK_CE_INBUF : STD_LOGIC; 
  signal CLK_BUFGP_BUFG_S_INVNOT : STD_LOGIC; 
  signal D0_OBUF_FFY_RST : STD_LOGIC; 
  signal D0_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D2_OBUF_FFY_RST : STD_LOGIC; 
  signal D2_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D1_OBUF_FFY_RST : STD_LOGIC; 
  signal D1_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal D3_OBUF_FFY_RST : STD_LOGIC; 
  signal D3_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal VCC_0 : STD_LOGIC; 
begin
  NlwRenamedSignal_VCC <= VCC;
  D0_OBUF_DYMUX_1 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_TQ_pack_1,
      O => D0_OBUF_DYMUX
    );
  D0_OBUF_CLKINV_2 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_BUFGP,
      O => D0_OBUF_CLKINV
    );
  D0_OBUF_CEINV_3 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D0_OBUF_CEINV
    );
  D2_OBUF_DYMUX_4 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_4_TQ_pack_1,
      O => D2_OBUF_DYMUX
    );
  D2_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D1_OBUF,
      O => D2_OBUF_CLKINVNOT
    );
  D2_OBUF_CEINV_5 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D2_OBUF_CEINV
    );
  D1_OBUF_DYMUX_6 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_TQ_pack_1,
      O => D1_OBUF_DYMUX
    );
  D1_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D0_OBUF,
      O => D1_OBUF_CLKINVNOT
    );
  D1_OBUF_CEINV_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D1_OBUF_CEINV
    );
  D3_OBUF_DYMUX_8 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_5_TQ_pack_1,
      O => D3_OBUF_DYMUX
    );
  D3_OBUF_CLKINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D2_OBUF,
      O => D3_OBUF_CLKINVNOT
    );
  D3_OBUF_CEINV_9 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLK_CE_IBUF,
      O => D3_OBUF_CEINV
    );
  VCC_IBUF_10 : X_BUF_PP
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
  D0_OBUF_11 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D0_O,
      CTL => D0_ENABLE,
      O => D0
    );
  D0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D0_ENABLE
    );
  D1_OBUF_12 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D1_O,
      CTL => D1_ENABLE,
      O => D1
    );
  D1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D1_ENABLE
    );
  D2_OBUF_13 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D2_O,
      CTL => D2_ENABLE,
      O => D2
    );
  D2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D2_ENABLE
    );
  D3_OBUF_14 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D3_O,
      CTL => D3_ENABLE,
      O => D3
    );
  D3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => D3_ENABLE
    );
  CLK_BUFGP_IBUFG_15 : X_BUF_PP
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
  CLR_IBUF_16 : X_BUF_PP
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
  CLK_CE_IBUF_17 : X_BUF_PP
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
      INIT => X"55AA"
    )
    port map (
      ADR0 => VCC_IBUF,
      ADR1 => VCC_0,
      ADR2 => VCC_0,
      ADR3 => D0_OBUF,
      O => XLXI_2_TQ_pack_1
    );
  XLXI_2_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D0_OBUF_DYMUX,
      CE => D0_OBUF_CEINV,
      CLK => D0_OBUF_CLKINV,
      SET => GND,
      RST => D0_OBUF_FFY_RST,
      O => D0_OBUF
    );
  D0_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D0_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D0_OBUF_FFY_RST
    );
  D0_OBUF_FFY_RSTAND_18 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D0_OBUF_FFY_RSTAND
    );
  XLXI_4_I_36_32 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => D2_OBUF,
      ADR2 => VCC_0,
      ADR3 => VCC_IBUF,
      O => XLXI_4_TQ_pack_1
    );
  XLXI_4_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D2_OBUF_DYMUX,
      CE => D2_OBUF_CEINV,
      CLK => D2_OBUF_CLKINVNOT,
      SET => GND,
      RST => D2_OBUF_FFY_RST,
      O => D2_OBUF
    );
  D2_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D2_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D2_OBUF_FFY_RST
    );
  D2_OBUF_FFY_RSTAND_19 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D2_OBUF_FFY_RSTAND
    );
  XLXI_3_I_36_32 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC_0,
      ADR1 => VCC_IBUF,
      ADR2 => VCC_0,
      ADR3 => D1_OBUF,
      O => XLXI_3_TQ_pack_1
    );
  XLXI_3_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D1_OBUF_DYMUX,
      CE => D1_OBUF_CEINV,
      CLK => D1_OBUF_CLKINVNOT,
      SET => GND,
      RST => D1_OBUF_FFY_RST,
      O => D1_OBUF
    );
  D1_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D1_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D1_OBUF_FFY_RST
    );
  D1_OBUF_FFY_RSTAND_20 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D1_OBUF_FFY_RSTAND
    );
  XLXI_5_I_36_32 : X_LUT4
    generic map(
      INIT => X"6666"
    )
    port map (
      ADR0 => VCC_IBUF,
      ADR1 => D3_OBUF,
      ADR2 => VCC_0,
      ADR3 => VCC_0,
      O => XLXI_5_TQ_pack_1
    );
  XLXI_5_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => D3_OBUF_DYMUX,
      CE => D3_OBUF_CEINV,
      CLK => D3_OBUF_CLKINVNOT,
      SET => GND,
      RST => D3_OBUF_FFY_RST,
      O => D3_OBUF
    );
  D3_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => D3_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => D3_OBUF_FFY_RST
    );
  D3_OBUF_FFY_RSTAND_21 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => CLR_IBUF,
      O => D3_OBUF_FFY_RSTAND
    );
  GLOBAL_LOGIC1_VCC : X_ONE
    port map (
      O => GLOBAL_LOGIC1
    );
  D0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D0_OBUF,
      O => D0_O
    );
  D1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D1_OBUF,
      O => D1_O
    );
  D2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D2_OBUF,
      O => D2_O
    );
  D3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => D3_OBUF,
      O => D3_O
    );
  NlwBlock_asyncounter_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_asyncounter_VCC : X_ONE
    port map (
      O => VCC_0
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

