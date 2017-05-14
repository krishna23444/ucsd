--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.39
--  \   \         Application: netgen
--  /   /         Filename: machine_timesim.vhd
-- /___/   /\     Timestamp: Wed Jun 01 14:08:20 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -s 5 -pcf machine.pcf -rpw 100 -tpw 0 -ar Structure -xon true -w -ofmt vhdl -sim machine.ncd machine_timesim.vhd 
-- Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
-- Input file	: machine.ncd
-- Output file	: machine_timesim.vhd
-- # of Entities	: 1
-- Design Name	: machine
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

entity machine is
  port (
    clk_en : in STD_LOGIC := 'X'; 
    clk : in STD_LOGIC := 'X'; 
    rst : in STD_LOGIC := 'X'; 
    OV_FLG : out STD_LOGIC; 
    GE_FLG : out STD_LOGIC; 
    R1 : out STD_LOGIC_VECTOR ( 4 downto 0 ); 
    R2 : out STD_LOGIC_VECTOR ( 4 downto 0 ); 
    R3 : out STD_LOGIC_VECTOR ( 4 downto 0 ); 
    pc : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    inst : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end machine;

architecture Structure of machine is
  signal GLOBAL_LOGIC0 : STD_LOGIC; 
  signal R1_0_OBUF : STD_LOGIC; 
  signal R2_0_OBUF : STD_LOGIC; 
  signal R1_1_OBUF : STD_LOGIC; 
  signal R2_1_OBUF : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I_36_55_O : STD_LOGIC; 
  signal R1_2_OBUF : STD_LOGIC; 
  signal R2_2_OBUF : STD_LOGIC; 
  signal R1_3_OBUF : STD_LOGIC; 
  signal R2_3_OBUF : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I_36_58_O : STD_LOGIC; 
  signal R1_4_OBUF : STD_LOGIC; 
  signal R2_4_OBUF : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_2_CO : STD_LOGIC; 
  signal clk_en_IBUF : STD_LOGIC; 
  signal clk_BUFGP : STD_LOGIC; 
  signal rst_IBUF : STD_LOGIC; 
  signal pc_0_OBUF : STD_LOGIC; 
  signal pc_1_OBUF : STD_LOGIC; 
  signal pc_3_OBUF : STD_LOGIC; 
  signal pc_2_OBUF : STD_LOGIC; 
  signal R3_2_OBUF : STD_LOGIC; 
  signal R3_3_OBUF : STD_LOGIC; 
  signal R3_4_OBUF : STD_LOGIC; 
  signal clk_BUFGP_IBUFG : STD_LOGIC; 
  signal XLXN_28_0 : STD_LOGIC; 
  signal inst_0_OBUF : STD_LOGIC; 
  signal inst_1_OBUF : STD_LOGIC; 
  signal inst_2_OBUF : STD_LOGIC; 
  signal inst_3_OBUF : STD_LOGIC; 
  signal inst_4_OBUF : STD_LOGIC; 
  signal inst_5_OBUF : STD_LOGIC; 
  signal inst_6_OBUF : STD_LOGIC; 
  signal inst_7_OBUF : STD_LOGIC; 
  signal R3_0_OBUF : STD_LOGIC; 
  signal R3_1_OBUF : STD_LOGIC; 
  signal GLOBAL_LOGIC1 : STD_LOGIC; 
  signal XLXN_29_0 : STD_LOGIC; 
  signal XLXN_30_0 : STD_LOGIC; 
  signal XLXN_31 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_I_36_3_1_0_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_LT0_1_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_EQ4_5_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_EQ2_3 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_GE2_3_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_LE2_3_0 : STD_LOGIC; 
  signal XLXN_32_0 : STD_LOGIC; 
  signal sel_1_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_MO2_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M01_0 : STD_LOGIC; 
  signal shf_of_1_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M23 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_I_M45_M0_0 : STD_LOGIC; 
  signal sel_0_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_MO1_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M12_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M34 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_MO0_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_LT2_3 : STD_LOGIC; 
  signal GSR : STD_LOGIC; 
  signal GTS : STD_LOGIC; 
  signal XLXI_1_S_0_XORF : STD_LOGIC; 
  signal XLXI_1_S_0_CYINIT : STD_LOGIC; 
  signal XLXI_1_S_0_CY0F : STD_LOGIC; 
  signal XLXI_1_S_0_CYSELF : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I0 : STD_LOGIC; 
  signal XLXI_1_S_0_XORG : STD_LOGIC; 
  signal XLXI_1_S_0_CYMUXG : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I_36_111_O : STD_LOGIC; 
  signal XLXI_1_S_0_CY0G : STD_LOGIC; 
  signal XLXI_1_S_0_CYSELG : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I1 : STD_LOGIC; 
  signal XLXI_1_S_2_XORF : STD_LOGIC; 
  signal XLXI_1_S_2_CYINIT : STD_LOGIC; 
  signal XLXI_1_S_2_CY0F : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I2 : STD_LOGIC; 
  signal XLXI_1_S_2_XORG : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I_36_62_O : STD_LOGIC; 
  signal XLXI_1_S_2_CYSELF : STD_LOGIC; 
  signal XLXI_1_S_2_CYMUXFAST : STD_LOGIC; 
  signal XLXI_1_S_2_CYAND : STD_LOGIC; 
  signal XLXI_1_S_2_FASTCARRY : STD_LOGIC; 
  signal XLXI_1_S_2_CYMUXG2 : STD_LOGIC; 
  signal XLXI_1_S_2_CYMUXF2 : STD_LOGIC; 
  signal XLXI_1_S_2_CY0G : STD_LOGIC; 
  signal XLXI_1_S_2_CYSELG : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I3 : STD_LOGIC; 
  signal XLXI_1_S_4_XORF : STD_LOGIC; 
  signal XLXI_1_S_4_CYINIT : STD_LOGIC; 
  signal XLXI_1_S_4_CY0F : STD_LOGIC; 
  signal XLXI_1_XLXI_2_I4 : STD_LOGIC; 
  signal XLXI_1_S_4_CYSELF : STD_LOGIC; 
  signal XLXI_1_S_4_CYMUXFAST : STD_LOGIC; 
  signal XLXI_1_S_4_CYAND : STD_LOGIC; 
  signal XLXI_1_S_4_FASTCARRY : STD_LOGIC; 
  signal XLXI_1_S_4_CYMUXG2 : STD_LOGIC; 
  signal XLXI_1_S_4_CYMUXF2 : STD_LOGIC; 
  signal XLXI_1_S_4_LOGIC_ZERO : STD_LOGIC; 
  signal XLXI_1_S_4_CYSELG : STD_LOGIC; 
  signal XLXI_1_S_4_G : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_CYINIT : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_CYSELF : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_F : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_CYMUXG : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_LOGIC_ZERO : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_CYSELG : STD_LOGIC; 
  signal XLXI_1_XLXI_2_C6O_G : STD_LOGIC; 
  signal pc_1_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_3_XLXI_1_I_Q1_TQ_pack_1 : STD_LOGIC; 
  signal pc_1_OBUF_CLKINV : STD_LOGIC; 
  signal pc_1_OBUF_CEINV : STD_LOGIC; 
  signal pc_3_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_3_XLXI_1_I_Q3_TQ_pack_1 : STD_LOGIC; 
  signal pc_3_OBUF_CLKINV : STD_LOGIC; 
  signal pc_3_OBUF_CEINV : STD_LOGIC; 
  signal pc_0_OBUF_DYMUX : STD_LOGIC; 
  signal pc_0_OBUF_CLKINV : STD_LOGIC; 
  signal pc_0_OBUF_CEINV : STD_LOGIC; 
  signal pc_2_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_3_XLXI_1_I_Q2_TQ_pack_1 : STD_LOGIC; 
  signal pc_2_OBUF_CLKINV : STD_LOGIC; 
  signal pc_2_OBUF_CEINV : STD_LOGIC; 
  signal R3_2_ENABLE : STD_LOGIC; 
  signal R3_2_O : STD_LOGIC; 
  signal R3_3_ENABLE : STD_LOGIC; 
  signal R3_3_O : STD_LOGIC; 
  signal R3_4_ENABLE : STD_LOGIC; 
  signal R3_4_O : STD_LOGIC; 
  signal clk_INBUF : STD_LOGIC; 
  signal OV_FLG_ENABLE : STD_LOGIC; 
  signal OV_FLG_O : STD_LOGIC; 
  signal OV_FLG_OUTPUT_OTCLK1INV : STD_LOGIC; 
  signal pc_0_ENABLE : STD_LOGIC; 
  signal pc_0_O : STD_LOGIC; 
  signal R2_0_OBUF_DXMUX : STD_LOGIC; 
  signal XLXI_2_XLXN_1_0_pack_1 : STD_LOGIC; 
  signal XLXN_31_pack_1 : STD_LOGIC; 
  signal R2_0_OBUF_SRFFMUX : STD_LOGIC; 
  signal R2_0_OBUF_CLKINV : STD_LOGIC; 
  signal R2_0_OBUF_CEINV : STD_LOGIC; 
  signal XLXI_1_XLXI_4_LT0_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_MO2 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_I_M45_M0 : STD_LOGIC; 
  signal R3_0_OBUF_DXMUX : STD_LOGIC; 
  signal XLXN_27_0_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_55_XLXN_19_0_pack_1 : STD_LOGIC; 
  signal R3_0_OBUF_SRFFMUX : STD_LOGIC; 
  signal R3_0_OBUF_CLKINV : STD_LOGIC; 
  signal R3_0_OBUF_CEINV : STD_LOGIC; 
  signal XLXN_26 : STD_LOGIC; 
  signal R3_3_OBUF_DXMUX : STD_LOGIC; 
  signal XLXN_27_3_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_55_XLXN_19_3_pack_1 : STD_LOGIC; 
  signal R3_3_OBUF_SRFFMUX : STD_LOGIC; 
  signal R3_3_OBUF_CLKINV : STD_LOGIC; 
  signal R3_3_OBUF_CEINV : STD_LOGIC; 
  signal XLXN_32 : STD_LOGIC; 
  signal XLXN_30 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_MO1 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M34_pack_1 : STD_LOGIC; 
  signal R3_4_OBUF_DXMUX : STD_LOGIC; 
  signal XLXN_27_4_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_55_XLXN_19_4_pack_1 : STD_LOGIC; 
  signal R3_4_OBUF_SRFFMUX : STD_LOGIC; 
  signal R3_4_OBUF_CLKINV : STD_LOGIC; 
  signal R3_4_OBUF_CEINV : STD_LOGIC; 
  signal XLXI_1_XLXI_3_MO0 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M23_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M12 : STD_LOGIC; 
  signal shf_of_0_pack_1 : STD_LOGIC; 
  signal inst_7_OBUF_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_I_36_3_1_0 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_LT2_3_pack_1 : STD_LOGIC; 
  signal R3_1_OBUF_DXMUX : STD_LOGIC; 
  signal XLXN_27_1_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_55_XLXN_19_1_pack_1 : STD_LOGIC; 
  signal R3_1_OBUF_SRFFMUX : STD_LOGIC; 
  signal R3_1_OBUF_CLKINV : STD_LOGIC; 
  signal R3_1_OBUF_CEINV : STD_LOGIC; 
  signal XLXN_24 : STD_LOGIC; 
  signal inst_6_OBUF_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_GE2_3 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_LE2_3 : STD_LOGIC; 
  signal R2_3_ENABLE : STD_LOGIC; 
  signal R2_3_O : STD_LOGIC; 
  signal R3_0_ENABLE : STD_LOGIC; 
  signal R3_0_O : STD_LOGIC; 
  signal R2_4_ENABLE : STD_LOGIC; 
  signal R2_4_O : STD_LOGIC; 
  signal R3_1_ENABLE : STD_LOGIC; 
  signal R3_1_O : STD_LOGIC; 
  signal clk_BUFGP_BUFG_S_INVNOT : STD_LOGIC; 
  signal R1_0_OBUF_DXMUX : STD_LOGIC; 
  signal inst_0_OBUF_pack_1 : STD_LOGIC; 
  signal R1_0_OBUF_DYMUX : STD_LOGIC; 
  signal inst_1_OBUF_pack_1 : STD_LOGIC; 
  signal R1_0_OBUF_SRFFMUX : STD_LOGIC; 
  signal R1_0_OBUF_CLKINV : STD_LOGIC; 
  signal R1_0_OBUF_CEINV : STD_LOGIC; 
  signal R1_4_OBUF_DXMUX : STD_LOGIC; 
  signal inst_4_OBUF_pack_1 : STD_LOGIC; 
  signal R1_4_OBUF_DYMUX : STD_LOGIC; 
  signal inst_2_OBUF_pack_1 : STD_LOGIC; 
  signal R1_4_OBUF_SRFFMUX : STD_LOGIC; 
  signal R1_4_OBUF_CLKINV : STD_LOGIC; 
  signal R1_4_OBUF_CEINV : STD_LOGIC; 
  signal R1_3_OBUF_DXMUX : STD_LOGIC; 
  signal inst_3_OBUF_pack_1 : STD_LOGIC; 
  signal R1_3_OBUF_SRFFMUX : STD_LOGIC; 
  signal R1_3_OBUF_CLKINV : STD_LOGIC; 
  signal R1_3_OBUF_CEINV : STD_LOGIC; 
  signal R2_2_OBUF_DXMUX : STD_LOGIC; 
  signal XLXI_2_XLXN_1_2_pack_1 : STD_LOGIC; 
  signal R2_2_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_2_XLXN_1_1_pack_1 : STD_LOGIC; 
  signal R2_2_OBUF_SRFFMUX : STD_LOGIC; 
  signal R2_2_OBUF_CLKINV : STD_LOGIC; 
  signal R2_2_OBUF_CEINV : STD_LOGIC; 
  signal R2_4_OBUF_DXMUX : STD_LOGIC; 
  signal XLXI_2_XLXN_1_4_pack_1 : STD_LOGIC; 
  signal R2_4_OBUF_DYMUX : STD_LOGIC; 
  signal XLXI_2_XLXN_1_3_pack_1 : STD_LOGIC; 
  signal R2_4_OBUF_SRFFMUX : STD_LOGIC; 
  signal R2_4_OBUF_CLKINV : STD_LOGIC; 
  signal R2_4_OBUF_CEINV : STD_LOGIC; 
  signal XLXN_23 : STD_LOGIC; 
  signal inst_5_OBUF_pack_1 : STD_LOGIC; 
  signal XLXN_25 : STD_LOGIC; 
  signal XLXI_1_XLXI_4_EQ2_3_pack_1 : STD_LOGIC; 
  signal XLXN_29 : STD_LOGIC; 
  signal XLXN_28 : STD_LOGIC; 
  signal R3_2_OBUF_DXMUX : STD_LOGIC; 
  signal XLXN_27_2_pack_1 : STD_LOGIC; 
  signal XLXI_1_XLXI_55_XLXN_19_2_pack_1 : STD_LOGIC; 
  signal R3_2_OBUF_SRFFMUX : STD_LOGIC; 
  signal R3_2_OBUF_CLKINV : STD_LOGIC; 
  signal R3_2_OBUF_CEINV : STD_LOGIC; 
  signal XLXI_1_XLXI_4_EQ4_5 : STD_LOGIC; 
  signal XLXI_1_XLXI_3_M01 : STD_LOGIC; 
  signal pc_1_ENABLE : STD_LOGIC; 
  signal pc_1_O : STD_LOGIC; 
  signal pc_2_ENABLE : STD_LOGIC; 
  signal pc_2_O : STD_LOGIC; 
  signal pc_3_ENABLE : STD_LOGIC; 
  signal pc_3_O : STD_LOGIC; 
  signal inst_0_ENABLE : STD_LOGIC; 
  signal inst_0_O : STD_LOGIC; 
  signal inst_1_ENABLE : STD_LOGIC; 
  signal inst_1_O : STD_LOGIC; 
  signal inst_2_ENABLE : STD_LOGIC; 
  signal inst_2_O : STD_LOGIC; 
  signal inst_3_ENABLE : STD_LOGIC; 
  signal inst_3_O : STD_LOGIC; 
  signal clk_en_INBUF : STD_LOGIC; 
  signal inst_4_ENABLE : STD_LOGIC; 
  signal inst_4_O : STD_LOGIC; 
  signal R1_0_ENABLE : STD_LOGIC; 
  signal R1_0_O : STD_LOGIC; 
  signal inst_5_ENABLE : STD_LOGIC; 
  signal inst_5_O : STD_LOGIC; 
  signal R1_1_ENABLE : STD_LOGIC; 
  signal R1_1_O : STD_LOGIC; 
  signal inst_6_ENABLE : STD_LOGIC; 
  signal inst_6_O : STD_LOGIC; 
  signal R1_2_ENABLE : STD_LOGIC; 
  signal R1_2_O : STD_LOGIC; 
  signal inst_7_ENABLE : STD_LOGIC; 
  signal inst_7_O : STD_LOGIC; 
  signal R1_3_ENABLE : STD_LOGIC; 
  signal R1_3_O : STD_LOGIC; 
  signal R2_0_ENABLE : STD_LOGIC; 
  signal R2_0_O : STD_LOGIC; 
  signal R1_4_ENABLE : STD_LOGIC; 
  signal R1_4_O : STD_LOGIC; 
  signal R2_1_ENABLE : STD_LOGIC; 
  signal R2_1_O : STD_LOGIC; 
  signal R2_2_ENABLE : STD_LOGIC; 
  signal R2_2_O : STD_LOGIC; 
  signal GE_FLG_ENABLE : STD_LOGIC; 
  signal GE_FLG_O : STD_LOGIC; 
  signal GE_FLG_OUTPUT_OTCLK1INV : STD_LOGIC; 
  signal rst_INBUF : STD_LOGIC; 
  signal OV_FLG_OBUF : STD_LOGIC; 
  signal OV_FLG_OUTPUT_OFF_OSR_USED : STD_LOGIC; 
  signal OV_FLG_OUTPUT_OFF_OCEINV : STD_LOGIC; 
  signal OV_FLG_OUTPUT_OFF_O1INV : STD_LOGIC; 
  signal GE_FLG_OBUF : STD_LOGIC; 
  signal GE_FLG_OUTPUT_OFF_OSR_USED : STD_LOGIC; 
  signal GE_FLG_OUTPUT_OFF_OCEINV : STD_LOGIC; 
  signal GE_FLG_OUTPUT_OFF_O1INV : STD_LOGIC; 
  signal pc_1_OBUF_FFY_RST : STD_LOGIC; 
  signal pc_1_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal pc_3_OBUF_FFY_RST : STD_LOGIC; 
  signal pc_3_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal pc_0_OBUF_FFY_RST : STD_LOGIC; 
  signal pc_0_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal pc_2_OBUF_FFY_RST : STD_LOGIC; 
  signal pc_2_OBUF_FFY_RSTAND : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal XLXI_1_S : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal XLXI_1_XLXI_55_XLXN_19 : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal shf_of : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal sel : STD_LOGIC_VECTOR ( 1 downto 0 ); 
begin
  XLXI_1_S_0_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_0_XORF,
      O => XLXI_1_S(0)
    );
  XLXI_1_S_0_XORF_0 : X_XOR2
    port map (
      I0 => XLXI_1_S_0_CYINIT,
      I1 => XLXI_1_XLXI_2_I0,
      O => XLXI_1_S_0_XORF
    );
  XLXI_1_S_0_CYMUXF : X_MUX2
    port map (
      IA => XLXI_1_S_0_CY0F,
      IB => XLXI_1_S_0_CYINIT,
      SEL => XLXI_1_S_0_CYSELF,
      O => XLXI_1_XLXI_2_I_36_111_O
    );
  XLXI_1_S_0_CYINIT_1 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GLOBAL_LOGIC0,
      O => XLXI_1_S_0_CYINIT
    );
  XLXI_1_S_0_CY0F_2 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_0_OBUF,
      O => XLXI_1_S_0_CY0F
    );
  XLXI_1_S_0_CYSELF_3 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I0,
      O => XLXI_1_S_0_CYSELF
    );
  XLXI_1_S_0_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_0_XORG,
      O => XLXI_1_S(1)
    );
  XLXI_1_S_0_XORG_4 : X_XOR2
    port map (
      I0 => XLXI_1_XLXI_2_I_36_111_O,
      I1 => XLXI_1_XLXI_2_I1,
      O => XLXI_1_S_0_XORG
    );
  XLXI_1_S_0_COUTUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_0_CYMUXG,
      O => XLXI_1_XLXI_2_I_36_55_O
    );
  XLXI_1_S_0_CYMUXG_5 : X_MUX2
    port map (
      IA => XLXI_1_S_0_CY0G,
      IB => XLXI_1_XLXI_2_I_36_111_O,
      SEL => XLXI_1_S_0_CYSELG,
      O => XLXI_1_S_0_CYMUXG
    );
  XLXI_1_S_0_CY0G_6 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_1_OBUF,
      O => XLXI_1_S_0_CY0G
    );
  XLXI_1_S_0_CYSELG_7 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I1,
      O => XLXI_1_S_0_CYSELG
    );
  XLXI_1_XLXI_2_I_36_229 : X_LUT4
    generic map(
      INIT => X"33CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => R1_1_OBUF,
      ADR2 => VCC,
      ADR3 => R2_1_OBUF,
      O => XLXI_1_XLXI_2_I1
    );
  XLXI_1_S_2_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_2_XORF,
      O => XLXI_1_S(2)
    );
  XLXI_1_S_2_XORF_8 : X_XOR2
    port map (
      I0 => XLXI_1_S_2_CYINIT,
      I1 => XLXI_1_XLXI_2_I2,
      O => XLXI_1_S_2_XORF
    );
  XLXI_1_S_2_CYMUXF2_9 : X_MUX2
    port map (
      IA => XLXI_1_S_2_CY0F,
      IB => XLXI_1_S_2_CY0F,
      SEL => XLXI_1_S_2_CYSELF,
      O => XLXI_1_S_2_CYMUXF2
    );
  XLXI_1_S_2_CYMUXF : X_MUX2
    port map (
      IA => XLXI_1_S_2_CY0F,
      IB => XLXI_1_S_2_CYINIT,
      SEL => XLXI_1_S_2_CYSELF,
      O => XLXI_1_XLXI_2_I_36_62_O
    );
  XLXI_1_S_2_CYINIT_10 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I_36_55_O,
      O => XLXI_1_S_2_CYINIT
    );
  XLXI_1_S_2_CY0F_11 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_2_OBUF,
      O => XLXI_1_S_2_CY0F
    );
  XLXI_1_S_2_CYSELF_12 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I2,
      O => XLXI_1_S_2_CYSELF
    );
  XLXI_1_S_2_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_2_XORG,
      O => XLXI_1_S(3)
    );
  XLXI_1_S_2_XORG_13 : X_XOR2
    port map (
      I0 => XLXI_1_XLXI_2_I_36_62_O,
      I1 => XLXI_1_XLXI_2_I3,
      O => XLXI_1_S_2_XORG
    );
  XLXI_1_S_2_COUTUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_2_CYMUXFAST,
      O => XLXI_1_XLXI_2_I_36_58_O
    );
  XLXI_1_S_2_FASTCARRY_14 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I_36_55_O,
      O => XLXI_1_S_2_FASTCARRY
    );
  XLXI_1_S_2_CYAND_15 : X_AND2
    port map (
      I0 => XLXI_1_S_2_CYSELG,
      I1 => XLXI_1_S_2_CYSELF,
      O => XLXI_1_S_2_CYAND
    );
  XLXI_1_S_2_CYMUXFAST_16 : X_MUX2
    port map (
      IA => XLXI_1_S_2_CYMUXG2,
      IB => XLXI_1_S_2_FASTCARRY,
      SEL => XLXI_1_S_2_CYAND,
      O => XLXI_1_S_2_CYMUXFAST
    );
  XLXI_1_S_2_CYMUXG2_17 : X_MUX2
    port map (
      IA => XLXI_1_S_2_CY0G,
      IB => XLXI_1_S_2_CYMUXF2,
      SEL => XLXI_1_S_2_CYSELG,
      O => XLXI_1_S_2_CYMUXG2
    );
  XLXI_1_S_2_CY0G_18 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_3_OBUF,
      O => XLXI_1_S_2_CY0G
    );
  XLXI_1_S_2_CYSELG_19 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I3,
      O => XLXI_1_S_2_CYSELG
    );
  XLXI_1_XLXI_2_I_36_225 : X_LUT4
    generic map(
      INIT => X"6666"
    )
    port map (
      ADR0 => R2_3_OBUF,
      ADR1 => R1_3_OBUF,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_XLXI_2_I3
    );
  XLXI_1_S_4_LOGIC_ZERO_20 : X_ZERO
    port map (
      O => XLXI_1_S_4_LOGIC_ZERO
    );
  XLXI_1_S_4_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_4_XORF,
      O => XLXI_1_S(4)
    );
  XLXI_1_S_4_XORF_21 : X_XOR2
    port map (
      I0 => XLXI_1_S_4_CYINIT,
      I1 => XLXI_1_XLXI_2_I4,
      O => XLXI_1_S_4_XORF
    );
  XLXI_1_S_4_CYMUXF2_22 : X_MUX2
    port map (
      IA => XLXI_1_S_4_CY0F,
      IB => XLXI_1_S_4_CY0F,
      SEL => XLXI_1_S_4_CYSELF,
      O => XLXI_1_S_4_CYMUXF2
    );
  XLXI_1_S_4_CYINIT_23 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I_36_58_O,
      O => XLXI_1_S_4_CYINIT
    );
  XLXI_1_S_4_CY0F_24 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_4_OBUF,
      O => XLXI_1_S_4_CY0F
    );
  XLXI_1_S_4_CYSELF_25 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I4,
      O => XLXI_1_S_4_CYSELF
    );
  XLXI_1_S_4_FASTCARRY_26 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_I_36_58_O,
      O => XLXI_1_S_4_FASTCARRY
    );
  XLXI_1_S_4_CYAND_27 : X_AND2
    port map (
      I0 => XLXI_1_S_4_CYSELG,
      I1 => XLXI_1_S_4_CYSELF,
      O => XLXI_1_S_4_CYAND
    );
  XLXI_1_S_4_CYMUXFAST_28 : X_MUX2
    port map (
      IA => XLXI_1_S_4_CYMUXG2,
      IB => XLXI_1_S_4_FASTCARRY,
      SEL => XLXI_1_S_4_CYAND,
      O => XLXI_1_S_4_CYMUXFAST
    );
  XLXI_1_S_4_CYMUXG2_29 : X_MUX2
    port map (
      IA => XLXI_1_S_4_LOGIC_ZERO,
      IB => XLXI_1_S_4_CYMUXF2,
      SEL => XLXI_1_S_4_CYSELG,
      O => XLXI_1_S_4_CYMUXG2
    );
  XLXI_1_S_4_CYSELG_30 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_4_G,
      O => XLXI_1_S_4_CYSELG
    );
  XLXI_1_XLXI_2_C6O_LOGIC_ZERO_31 : X_ZERO
    port map (
      O => XLXI_1_XLXI_2_C6O_LOGIC_ZERO
    );
  XLXI_1_XLXI_2_C6O_XBMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_C6O,
      O => XLXI_1_XLXI_2_C6O_0
    );
  XLXI_1_XLXI_2_C6O_CYMUXF : X_MUX2
    port map (
      IA => XLXI_1_XLXI_2_C6O_LOGIC_ZERO,
      IB => XLXI_1_XLXI_2_C6O_CYINIT,
      SEL => XLXI_1_XLXI_2_C6O_CYSELF,
      O => XLXI_1_XLXI_2_C6O
    );
  XLXI_1_XLXI_2_C6O_CYINIT_32 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_S_4_CYMUXFAST,
      O => XLXI_1_XLXI_2_C6O_CYINIT
    );
  XLXI_1_XLXI_2_C6O_CYSELF_33 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_C6O_F,
      O => XLXI_1_XLXI_2_C6O_CYSELF
    );
  XLXI_1_XLXI_2_C6O_YBMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_C6O_CYMUXG,
      O => XLXI_1_XLXI_2_CO
    );
  XLXI_1_XLXI_2_C6O_CYMUXG_34 : X_MUX2
    port map (
      IA => XLXI_1_XLXI_2_C6O_LOGIC_ZERO,
      IB => XLXI_1_XLXI_2_C6O,
      SEL => XLXI_1_XLXI_2_C6O_CYSELG,
      O => XLXI_1_XLXI_2_C6O_CYMUXG
    );
  XLXI_1_XLXI_2_C6O_CYSELG_35 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_2_C6O_G,
      O => XLXI_1_XLXI_2_C6O_CYSELG
    );
  pc_1_OBUF_DYMUX_36 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_1_I_Q1_TQ_pack_1,
      O => pc_1_OBUF_DYMUX
    );
  pc_1_OBUF_CLKINV_37 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => pc_1_OBUF_CLKINV
    );
  pc_1_OBUF_CEINV_38 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_en_IBUF,
      O => pc_1_OBUF_CEINV
    );
  pc_3_OBUF_DYMUX_39 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_1_I_Q3_TQ_pack_1,
      O => pc_3_OBUF_DYMUX
    );
  pc_3_OBUF_CLKINV_40 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => pc_3_OBUF_CLKINV
    );
  pc_3_OBUF_CEINV_41 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_en_IBUF,
      O => pc_3_OBUF_CEINV
    );
  pc_0_OBUF_DYMUX_42 : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_0_OBUF,
      O => pc_0_OBUF_DYMUX
    );
  pc_0_OBUF_CLKINV_43 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => pc_0_OBUF_CLKINV
    );
  pc_0_OBUF_CEINV_44 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_en_IBUF,
      O => pc_0_OBUF_CEINV
    );
  pc_2_OBUF_DYMUX_45 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_3_XLXI_1_I_Q2_TQ_pack_1,
      O => pc_2_OBUF_DYMUX
    );
  pc_2_OBUF_CLKINV_46 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => pc_2_OBUF_CLKINV
    );
  pc_2_OBUF_CEINV_47 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_en_IBUF,
      O => pc_2_OBUF_CEINV
    );
  R3_2_OBUF_48 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_2_O,
      CTL => R3_2_ENABLE,
      O => R3(2)
    );
  R3_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R3_2_ENABLE
    );
  R3_3_OBUF_49 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_3_O,
      CTL => R3_3_ENABLE,
      O => R3(3)
    );
  R3_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R3_3_ENABLE
    );
  R3_4_OBUF_50 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_4_O,
      CTL => R3_4_ENABLE,
      O => R3(4)
    );
  R3_4_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R3_4_ENABLE
    );
  clk_BUFGP_IBUFG_51 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk,
      O => clk_INBUF
    );
  clk_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_INBUF,
      O => clk_BUFGP_IBUFG
    );
  OV_FLG_OBUF_52 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => OV_FLG_O,
      CTL => OV_FLG_ENABLE,
      O => OV_FLG
    );
  OV_FLG_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => OV_FLG_ENABLE
    );
  OV_FLG_OUTPUT_OTCLK1INV_53 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => OV_FLG_OUTPUT_OTCLK1INV
    );
  pc_0_OBUF_54 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_0_O,
      CTL => pc_0_ENABLE,
      O => pc(0)
    );
  pc_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => pc_0_ENABLE
    );
  R2_0_OBUF_DXMUX_55 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_1_0_pack_1,
      O => R2_0_OBUF_DXMUX
    );
  R2_0_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_31_pack_1,
      O => XLXN_31
    );
  R2_0_OBUF_SRFFMUX_56 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R2_0_OBUF_SRFFMUX
    );
  R2_0_OBUF_CLKINV_57 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R2_0_OBUF_CLKINV
    );
  R2_0_OBUF_CEINV_58 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_30_0,
      O => R2_0_OBUF_CEINV
    );
  XLXI_1_XLXI_4_LT0_1_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_LT0_1,
      O => XLXI_1_XLXI_4_LT0_1_0
    );
  XLXI_1_XLXI_4_I_36_18 : X_LUT4
    generic map(
      INIT => X"08CE"
    )
    port map (
      ADR0 => R2_0_OBUF,
      ADR1 => R2_1_OBUF,
      ADR2 => R1_0_OBUF,
      ADR3 => R1_1_OBUF,
      O => XLXI_1_XLXI_4_LT0_1
    );
  XLXI_1_XLXI_3_MO2_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_MO2,
      O => XLXI_1_XLXI_3_MO2_0
    );
  XLXI_1_XLXI_3_MO2_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_I_M45_M0,
      O => XLXI_1_XLXI_3_I_M45_M0_0
    );
  XLXI_1_XLXI_3_I_M45_I_36_7 : X_LUT4
    generic map(
      INIT => X"4444"
    )
    port map (
      ADR0 => shf_of(0),
      ADR1 => R2_0_OBUF,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_XLXI_3_I_M45_M0
    );
  R3_0_OBUF_DXMUX_59 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_27_0_pack_1,
      O => R3_0_OBUF_DXMUX
    );
  R3_0_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_55_XLXN_19_0_pack_1,
      O => XLXI_1_XLXI_55_XLXN_19(0)
    );
  R3_0_OBUF_SRFFMUX_60 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R3_0_OBUF_SRFFMUX
    );
  R3_0_OBUF_CLKINV_61 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R3_0_OBUF_CLKINV
    );
  R3_0_OBUF_CEINV_62 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_32_0,
      O => R3_0_OBUF_CEINV
    );
  XLXI_1_XLXI_2_I_36_239 : X_LUT4
    generic map(
      INIT => X"0FF0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => XLXI_1_XLXI_2_CO,
      ADR3 => XLXI_1_XLXI_2_C6O_0,
      O => XLXN_26
    );
  R3_3_OBUF_DXMUX_63 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_27_3_pack_1,
      O => R3_3_OBUF_DXMUX
    );
  R3_3_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_55_XLXN_19_3_pack_1,
      O => XLXI_1_XLXI_55_XLXN_19(3)
    );
  R3_3_OBUF_SRFFMUX_64 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R3_3_OBUF_SRFFMUX
    );
  R3_3_OBUF_CLKINV_65 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R3_3_OBUF_CLKINV
    );
  R3_3_OBUF_CEINV_66 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_32_0,
      O => R3_3_OBUF_CEINV
    );
  XLXN_32_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_32,
      O => XLXN_32_0
    );
  XLXN_32_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => sel(0),
      O => sel_0_0
    );
  sel_0_1 : X_LUT4
    generic map(
      INIT => X"3030"
    )
    port map (
      ADR0 => VCC,
      ADR1 => inst_5_OBUF,
      ADR2 => inst_7_OBUF,
      ADR3 => VCC,
      O => sel(0)
    );
  XLXN_30_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_30,
      O => XLXN_30_0
    );
  XLXN_30_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => sel(1),
      O => sel_1_0
    );
  sel_1_1 : X_LUT4
    generic map(
      INIT => X"CC00"
    )
    port map (
      ADR0 => VCC,
      ADR1 => inst_7_OBUF,
      ADR2 => VCC,
      ADR3 => inst_6_OBUF,
      O => sel(1)
    );
  XLXI_1_XLXI_3_MO1_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_MO1,
      O => XLXI_1_XLXI_3_MO1_0
    );
  XLXI_1_XLXI_3_MO1_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_M34_pack_1,
      O => XLXI_1_XLXI_3_M34
    );
  XLXI_1_XLXI_3_I_M34_I_36_8 : X_LUT4
    generic map(
      INIT => X"F5A0"
    )
    port map (
      ADR0 => shf_of(0),
      ADR1 => VCC,
      ADR2 => R2_0_OBUF,
      ADR3 => R2_1_OBUF,
      O => XLXI_1_XLXI_3_M34_pack_1
    );
  R3_4_OBUF_DXMUX_67 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_27_4_pack_1,
      O => R3_4_OBUF_DXMUX
    );
  R3_4_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_55_XLXN_19_4_pack_1,
      O => XLXI_1_XLXI_55_XLXN_19(4)
    );
  R3_4_OBUF_SRFFMUX_68 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R3_4_OBUF_SRFFMUX
    );
  R3_4_OBUF_CLKINV_69 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R3_4_OBUF_CLKINV
    );
  R3_4_OBUF_CEINV_70 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_32_0,
      O => R3_4_OBUF_CEINV
    );
  XLXI_1_XLXI_3_MO0_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_MO0,
      O => XLXI_1_XLXI_3_MO0_0
    );
  XLXI_1_XLXI_3_MO0_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_M23_pack_1,
      O => XLXI_1_XLXI_3_M23
    );
  XLXI_1_XLXI_3_I_M23_I_36_8 : X_LUT4
    generic map(
      INIT => X"FA50"
    )
    port map (
      ADR0 => shf_of(0),
      ADR1 => VCC,
      ADR2 => R2_2_OBUF,
      ADR3 => R2_1_OBUF,
      O => XLXI_1_XLXI_3_M23_pack_1
    );
  XLXI_1_XLXI_3_M12_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_M12,
      O => XLXI_1_XLXI_3_M12_0
    );
  XLXI_1_XLXI_3_M12_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => shf_of_0_pack_1,
      O => shf_of(0)
    );
  shf_of_0_1 : X_LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      ADR0 => inst_7_OBUF,
      ADR1 => inst_3_OBUF,
      ADR2 => inst_5_OBUF,
      ADR3 => inst_6_OBUF,
      O => shf_of_0_pack_1
    );
  shf_of_1_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => shf_of(1),
      O => shf_of_1_0
    );
  shf_of_1_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_7_OBUF_pack_1,
      O => inst_7_OBUF
    );
  XLXI_4_XLXI_1 : X_LUT4
    generic map(
      INIT => X"4438"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_1_OBUF,
      ADR2 => pc_3_OBUF,
      ADR3 => pc_0_OBUF,
      O => inst_7_OBUF_pack_1
    );
  XLXI_1_XLXI_4_I_36_3_1_0_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_I_36_3_1_0,
      O => XLXI_1_XLXI_4_I_36_3_1_0_0
    );
  XLXI_1_XLXI_4_I_36_3_1_0_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_LT2_3_pack_1,
      O => XLXI_1_XLXI_4_LT2_3
    );
  XLXI_1_XLXI_4_I_36_6 : X_LUT4
    generic map(
      INIT => X"0C8E"
    )
    port map (
      ADR0 => R2_2_OBUF,
      ADR1 => R2_3_OBUF,
      ADR2 => R1_3_OBUF,
      ADR3 => R1_2_OBUF,
      O => XLXI_1_XLXI_4_LT2_3_pack_1
    );
  R3_1_OBUF_DXMUX_71 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_27_1_pack_1,
      O => R3_1_OBUF_DXMUX
    );
  R3_1_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_55_XLXN_19_1_pack_1,
      O => XLXI_1_XLXI_55_XLXN_19(1)
    );
  R3_1_OBUF_SRFFMUX_72 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R3_1_OBUF_SRFFMUX
    );
  R3_1_OBUF_CLKINV_73 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R3_1_OBUF_CLKINV
    );
  R3_1_OBUF_CEINV_74 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_32_0,
      O => R3_1_OBUF_CEINV
    );
  XLXN_24_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_6_OBUF_pack_1,
      O => inst_6_OBUF
    );
  XLXI_4_XLXI_9 : X_LUT4
    generic map(
      INIT => X"1C92"
    )
    port map (
      ADR0 => pc_1_OBUF,
      ADR1 => pc_0_OBUF,
      ADR2 => pc_2_OBUF,
      ADR3 => pc_3_OBUF,
      O => inst_6_OBUF_pack_1
    );
  XLXI_1_XLXI_4_GE2_3_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_GE2_3,
      O => XLXI_1_XLXI_4_GE2_3_0
    );
  XLXI_1_XLXI_4_GE2_3_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_LE2_3,
      O => XLXI_1_XLXI_4_LE2_3_0
    );
  XLXI_1_XLXI_4_I_36_11 : X_LUT4
    generic map(
      INIT => X"2010"
    )
    port map (
      ADR0 => R1_3_OBUF,
      ADR1 => R1_2_OBUF,
      ADR2 => R2_2_OBUF,
      ADR3 => R2_3_OBUF,
      O => XLXI_1_XLXI_4_LE2_3
    );
  R2_3_OBUF_75 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_3_O,
      CTL => R2_3_ENABLE,
      O => R2(3)
    );
  R2_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R2_3_ENABLE
    );
  R3_0_OBUF_76 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_0_O,
      CTL => R3_0_ENABLE,
      O => R3(0)
    );
  R3_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R3_0_ENABLE
    );
  R2_4_OBUF_77 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_4_O,
      CTL => R2_4_ENABLE,
      O => R2(4)
    );
  R2_4_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R2_4_ENABLE
    );
  R3_1_OBUF_78 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_1_O,
      CTL => R3_1_ENABLE,
      O => R3(1)
    );
  R3_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R3_1_ENABLE
    );
  clk_BUFGP_BUFG : X_BUFGMUX
    port map (
      I0 => clk_BUFGP_IBUFG,
      I1 => GND,
      S => clk_BUFGP_BUFG_S_INVNOT,
      O => clk_BUFGP,
      GSR => GSR
    );
  clk_BUFGP_BUFG_SINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GLOBAL_LOGIC1,
      O => clk_BUFGP_BUFG_S_INVNOT
    );
  R1_0_OBUF_DXMUX_79 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_0_OBUF,
      O => R1_0_OBUF_DXMUX
    );
  R1_0_OBUF_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_0_OBUF_pack_1,
      O => inst_0_OBUF
    );
  R1_0_OBUF_DYMUX_80 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_1_OBUF,
      O => R1_0_OBUF_DYMUX
    );
  R1_0_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_1_OBUF_pack_1,
      O => inst_1_OBUF
    );
  R1_0_OBUF_SRFFMUX_81 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R1_0_OBUF_SRFFMUX
    );
  R1_0_OBUF_CLKINV_82 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R1_0_OBUF_CLKINV
    );
  R1_0_OBUF_CEINV_83 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_29_0,
      O => R1_0_OBUF_CEINV
    );
  R1_4_OBUF_DXMUX_84 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_4_OBUF,
      O => R1_4_OBUF_DXMUX
    );
  R1_4_OBUF_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_4_OBUF_pack_1,
      O => inst_4_OBUF
    );
  R1_4_OBUF_DYMUX_85 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_2_OBUF,
      O => R1_4_OBUF_DYMUX
    );
  R1_4_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_2_OBUF_pack_1,
      O => inst_2_OBUF
    );
  R1_4_OBUF_SRFFMUX_86 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R1_4_OBUF_SRFFMUX
    );
  R1_4_OBUF_CLKINV_87 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R1_4_OBUF_CLKINV
    );
  R1_4_OBUF_CEINV_88 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_29_0,
      O => R1_4_OBUF_CEINV
    );
  XLXI_4_XLXI_19 : X_LUT4
    generic map(
      INIT => X"0144"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_1_OBUF,
      ADR2 => pc_3_OBUF,
      ADR3 => pc_0_OBUF,
      O => inst_2_OBUF_pack_1
    );
  R1_3_OBUF_DXMUX_89 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_3_OBUF,
      O => R1_3_OBUF_DXMUX
    );
  R1_3_OBUF_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_3_OBUF_pack_1,
      O => inst_3_OBUF
    );
  R1_3_OBUF_SRFFMUX_90 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R1_3_OBUF_SRFFMUX
    );
  R1_3_OBUF_CLKINV_91 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R1_3_OBUF_CLKINV
    );
  R1_3_OBUF_CEINV_92 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_29_0,
      O => R1_3_OBUF_CEINV
    );
  R2_2_OBUF_DXMUX_93 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_1_2_pack_1,
      O => R2_2_OBUF_DXMUX
    );
  R2_2_OBUF_DYMUX_94 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_1_1_pack_1,
      O => R2_2_OBUF_DYMUX
    );
  R2_2_OBUF_SRFFMUX_95 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R2_2_OBUF_SRFFMUX
    );
  R2_2_OBUF_CLKINV_96 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R2_2_OBUF_CLKINV
    );
  R2_2_OBUF_CEINV_97 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_30_0,
      O => R2_2_OBUF_CEINV
    );
  R2_4_OBUF_DXMUX_98 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_1_4_pack_1,
      O => R2_4_OBUF_DXMUX
    );
  R2_4_OBUF_DYMUX_99 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_2_XLXN_1_3_pack_1,
      O => R2_4_OBUF_DYMUX
    );
  R2_4_OBUF_SRFFMUX_100 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R2_4_OBUF_SRFFMUX
    );
  R2_4_OBUF_CLKINV_101 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R2_4_OBUF_CLKINV
    );
  R2_4_OBUF_CEINV_102 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_30_0,
      O => R2_4_OBUF_CEINV
    );
  XLXN_23_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_5_OBUF_pack_1,
      O => inst_5_OBUF
    );
  XLXI_4_XLXI_14 : X_LUT4
    generic map(
      INIT => X"427E"
    )
    port map (
      ADR0 => pc_3_OBUF,
      ADR1 => pc_2_OBUF,
      ADR2 => pc_0_OBUF,
      ADR3 => pc_1_OBUF,
      O => inst_5_OBUF_pack_1
    );
  XLXN_25_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_EQ2_3_pack_1,
      O => XLXI_1_XLXI_4_EQ2_3
    );
  XLXI_1_XLXI_4_I_36_39 : X_LUT4
    generic map(
      INIT => X"0201"
    )
    port map (
      ADR0 => R2_3_OBUF,
      ADR1 => XLXI_1_XLXI_4_LE2_3_0,
      ADR2 => XLXI_1_XLXI_4_GE2_3_0,
      ADR3 => R1_3_OBUF,
      O => XLXI_1_XLXI_4_EQ2_3_pack_1
    );
  XLXN_29_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_29,
      O => XLXN_29_0
    );
  XLXN_29_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28,
      O => XLXN_28_0
    );
  XLXN_281 : X_LUT4
    generic map(
      INIT => X"0011"
    )
    port map (
      ADR0 => inst_5_OBUF,
      ADR1 => inst_7_OBUF,
      ADR2 => VCC,
      ADR3 => inst_6_OBUF,
      O => XLXN_28
    );
  R3_2_OBUF_DXMUX_103 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_27_2_pack_1,
      O => R3_2_OBUF_DXMUX
    );
  R3_2_OBUF_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_55_XLXN_19_2_pack_1,
      O => XLXI_1_XLXI_55_XLXN_19(2)
    );
  R3_2_OBUF_SRFFMUX_104 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => R3_2_OBUF_SRFFMUX
    );
  R3_2_OBUF_CLKINV_105 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => R3_2_OBUF_CLKINV
    );
  R3_2_OBUF_CEINV_106 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_32_0,
      O => R3_2_OBUF_CEINV
    );
  XLXI_1_XLXI_4_EQ4_5_XUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_4_EQ4_5,
      O => XLXI_1_XLXI_4_EQ4_5_0
    );
  XLXI_1_XLXI_4_EQ4_5_YUSED : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXI_1_XLXI_3_M01,
      O => XLXI_1_XLXI_3_M01_0
    );
  XLXI_1_XLXI_3_I_M01_I_36_8 : X_LUT4
    generic map(
      INIT => X"F0AA"
    )
    port map (
      ADR0 => R2_4_OBUF,
      ADR1 => VCC,
      ADR2 => R2_3_OBUF,
      ADR3 => shf_of(0),
      O => XLXI_1_XLXI_3_M01
    );
  pc_1_OBUF_107 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_1_O,
      CTL => pc_1_ENABLE,
      O => pc(1)
    );
  pc_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => pc_1_ENABLE
    );
  pc_2_OBUF_108 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_2_O,
      CTL => pc_2_ENABLE,
      O => pc(2)
    );
  pc_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => pc_2_ENABLE
    );
  pc_3_OBUF_109 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_3_O,
      CTL => pc_3_ENABLE,
      O => pc(3)
    );
  pc_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => pc_3_ENABLE
    );
  inst_0_OBUF_110 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_0_O,
      CTL => inst_0_ENABLE,
      O => inst(0)
    );
  inst_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_0_ENABLE
    );
  inst_1_OBUF_111 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_1_O,
      CTL => inst_1_ENABLE,
      O => inst(1)
    );
  inst_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_1_ENABLE
    );
  inst_2_OBUF_112 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_2_O,
      CTL => inst_2_ENABLE,
      O => inst(2)
    );
  inst_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_2_ENABLE
    );
  inst_3_OBUF_113 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_3_O,
      CTL => inst_3_ENABLE,
      O => inst(3)
    );
  inst_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_3_ENABLE
    );
  clk_en_IBUF_114 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_en,
      O => clk_en_INBUF
    );
  clk_en_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_en_INBUF,
      O => clk_en_IBUF
    );
  inst_4_OBUF_115 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_4_O,
      CTL => inst_4_ENABLE,
      O => inst(4)
    );
  inst_4_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_4_ENABLE
    );
  R1_0_OBUF_116 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_0_O,
      CTL => R1_0_ENABLE,
      O => R1(0)
    );
  R1_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R1_0_ENABLE
    );
  inst_5_OBUF_117 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_5_O,
      CTL => inst_5_ENABLE,
      O => inst(5)
    );
  inst_5_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_5_ENABLE
    );
  R1_1_OBUF_118 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_1_O,
      CTL => R1_1_ENABLE,
      O => R1(1)
    );
  R1_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R1_1_ENABLE
    );
  inst_6_OBUF_119 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_6_O,
      CTL => inst_6_ENABLE,
      O => inst(6)
    );
  inst_6_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_6_ENABLE
    );
  R1_2_OBUF_120 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_2_O,
      CTL => R1_2_ENABLE,
      O => R1(2)
    );
  R1_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R1_2_ENABLE
    );
  inst_7_OBUF_121 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_7_O,
      CTL => inst_7_ENABLE,
      O => inst(7)
    );
  inst_7_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => inst_7_ENABLE
    );
  R1_3_OBUF_122 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_3_O,
      CTL => R1_3_ENABLE,
      O => R1(3)
    );
  R1_3_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R1_3_ENABLE
    );
  R2_0_OBUF_123 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_0_O,
      CTL => R2_0_ENABLE,
      O => R2(0)
    );
  R2_0_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R2_0_ENABLE
    );
  R1_4_OBUF_124 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_4_O,
      CTL => R1_4_ENABLE,
      O => R1(4)
    );
  R1_4_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R1_4_ENABLE
    );
  R2_1_OBUF_125 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_1_O,
      CTL => R2_1_ENABLE,
      O => R2(1)
    );
  R2_1_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R2_1_ENABLE
    );
  R2_2_OBUF_126 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_2_O,
      CTL => R2_2_ENABLE,
      O => R2(2)
    );
  R2_2_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => R2_2_ENABLE
    );
  GE_FLG_OBUF_127 : X_TRI_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GE_FLG_O,
      CTL => GE_FLG_ENABLE,
      O => GE_FLG
    );
  GE_FLG_ENABLEINV : X_INV_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GTS,
      O => GE_FLG_ENABLE
    );
  GE_FLG_OUTPUT_OTCLK1INV_128 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => clk_BUFGP,
      O => GE_FLG_OUTPUT_OTCLK1INV
    );
  rst_IFF_IMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => rst_INBUF,
      O => rst_IBUF
    );
  rst_IBUF_129 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => rst,
      O => rst_INBUF
    );
  XLXI_2_XLXI_4_XLXI_4_I_36_8 : X_LUT4
    generic map(
      INIT => X"F0CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => inst_1_OBUF,
      ADR2 => R3_1_OBUF,
      ADR3 => XLXN_31,
      O => XLXI_2_XLXN_1_1_pack_1
    );
  XLXI_2_XLXI_4_XLXI_6_I_36_8 : X_LUT4
    generic map(
      INIT => X"BB88"
    )
    port map (
      ADR0 => R3_3_OBUF,
      ADR1 => XLXN_31,
      ADR2 => VCC,
      ADR3 => inst_3_OBUF,
      O => XLXI_2_XLXN_1_3_pack_1
    );
  XLXI_1_XLXI_55_XLXI_1_XLXI_4_I_36_8 : X_LUT4
    generic map(
      INIT => X"CC50"
    )
    port map (
      ADR0 => shf_of_1_0,
      ADR1 => XLXI_1_S(1),
      ADR2 => XLXI_1_XLXI_3_M34,
      ADR3 => sel_0_0,
      O => XLXI_1_XLXI_55_XLXN_19_1_pack_1
    );
  XLXI_1_XLXI_2_I_36_228 : X_LUT4
    generic map(
      INIT => X"6666"
    )
    port map (
      ADR0 => R1_0_OBUF,
      ADR1 => R2_0_OBUF,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_XLXI_2_I0
    );
  OV_FLG_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => OV_FLG_OBUF,
      O => OV_FLG_O
    );
  OV_FLG_OUTPUT_OFF_OSR_USED_130 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => OV_FLG_OUTPUT_OFF_OSR_USED
    );
  OV_FLG_OUTPUT_OFF_OCEINV_131 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_24,
      O => OV_FLG_OUTPUT_OFF_OCEINV
    );
  OV_FLG_OUTPUT_OFF_O1INV_132 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_26,
      O => OV_FLG_OUTPUT_OFF_O1INV
    );
  GE_FLG_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => GE_FLG_OBUF,
      O => GE_FLG_O
    );
  GE_FLG_OUTPUT_OFF_OSR_USED_133 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_28_0,
      O => GE_FLG_OUTPUT_OFF_OSR_USED
    );
  GE_FLG_OUTPUT_OFF_OCEINV_134 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_23,
      O => GE_FLG_OUTPUT_OFF_OCEINV
    );
  GE_FLG_OUTPUT_OFF_O1INV_135 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => XLXN_25,
      O => GE_FLG_OUTPUT_OFF_O1INV
    );
  XLXI_4_XLXI_18 : X_LUT4
    generic map(
      INIT => X"0050"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_3_OBUF,
      ADR2 => pc_1_OBUF,
      ADR3 => pc_0_OBUF,
      O => inst_1_OBUF_pack_1
    );
  XLXI_2_Reg1_XLXI_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R1_0_OBUF_DYMUX,
      CE => R1_0_OBUF_CEINV,
      CLK => R1_0_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R1_0_OBUF_SRFFMUX,
      O => R1_1_OBUF
    );
  XLXI_4_XLXI_16 : X_LUT4
    generic map(
      INIT => X"0340"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_3_OBUF,
      ADR2 => pc_1_OBUF,
      ADR3 => pc_0_OBUF,
      O => inst_0_OBUF_pack_1
    );
  XLXI_2_Reg1_XLXI_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R1_0_OBUF_DXMUX,
      CE => R1_0_OBUF_CEINV,
      CLK => R1_0_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R1_0_OBUF_SRFFMUX,
      O => R1_0_OBUF
    );
  XLXI_2_Reg1_XLXI_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R1_4_OBUF_DYMUX,
      CE => R1_4_OBUF_CEINV,
      CLK => R1_4_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R1_4_OBUF_SRFFMUX,
      O => R1_2_OBUF
    );
  XLXI_4_XLXI_15 : X_LUT4
    generic map(
      INIT => X"0110"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_1_OBUF,
      ADR2 => pc_3_OBUF,
      ADR3 => pc_0_OBUF,
      O => inst_4_OBUF_pack_1
    );
  XLXI_2_Reg1_XLXI_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R1_4_OBUF_DXMUX,
      CE => R1_4_OBUF_CEINV,
      CLK => R1_4_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R1_4_OBUF_SRFFMUX,
      O => R1_4_OBUF
    );
  XLXI_4_XLXI_20 : X_LUT4
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => pc_0_OBUF,
      ADR1 => pc_3_OBUF,
      ADR2 => pc_1_OBUF,
      ADR3 => pc_2_OBUF,
      O => inst_3_OBUF_pack_1
    );
  XLXI_2_Reg1_XLXI_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R1_3_OBUF_DXMUX,
      CE => R1_3_OBUF_CEINV,
      CLK => R1_3_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R1_3_OBUF_SRFFMUX,
      O => R1_3_OBUF
    );
  XLXI_2_Reg2_XLXI_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R2_2_OBUF_DYMUX,
      CE => R2_2_OBUF_CEINV,
      CLK => R2_2_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R2_2_OBUF_SRFFMUX,
      O => R2_1_OBUF
    );
  XLXI_2_XLXI_4_XLXI_5_I_36_8 : X_LUT4
    generic map(
      INIT => X"AACC"
    )
    port map (
      ADR0 => R3_2_OBUF,
      ADR1 => inst_2_OBUF,
      ADR2 => VCC,
      ADR3 => XLXN_31,
      O => XLXI_2_XLXN_1_2_pack_1
    );
  XLXI_2_Reg2_XLXI_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R2_2_OBUF_DXMUX,
      CE => R2_2_OBUF_CEINV,
      CLK => R2_2_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R2_2_OBUF_SRFFMUX,
      O => R2_2_OBUF
    );
  XLXI_2_Reg2_XLXI_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R2_4_OBUF_DYMUX,
      CE => R2_4_OBUF_CEINV,
      CLK => R2_4_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R2_4_OBUF_SRFFMUX,
      O => R2_3_OBUF
    );
  XLXI_1_XLXI_2_I_36_230 : X_LUT4
    generic map(
      INIT => X"55AA"
    )
    port map (
      ADR0 => R1_2_OBUF,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => R2_2_OBUF,
      O => XLXI_1_XLXI_2_I2
    );
  XLXI_1_XLXI_2_I_36_224 : X_LUT4
    generic map(
      INIT => X"6666"
    )
    port map (
      ADR0 => R1_4_OBUF,
      ADR1 => R2_4_OBUF,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_XLXI_2_I4
    );
  XLXI_3_XLXI_1_I_Q1_I_36_32 : X_LUT4
    generic map(
      INIT => X"0FF0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => pc_1_OBUF,
      ADR3 => pc_0_OBUF,
      O => XLXI_3_XLXI_1_I_Q1_TQ_pack_1
    );
  XLXI_3_XLXI_1_I_Q1_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => pc_1_OBUF_DYMUX,
      CE => pc_1_OBUF_CEINV,
      CLK => pc_1_OBUF_CLKINV,
      SET => GND,
      RST => pc_1_OBUF_FFY_RST,
      O => pc_1_OBUF
    );
  pc_1_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => pc_1_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => pc_1_OBUF_FFY_RST
    );
  pc_1_OBUF_FFY_RSTAND_136 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => rst_IBUF,
      O => pc_1_OBUF_FFY_RSTAND
    );
  XLXI_3_XLXI_1_I_Q3_I_36_32 : X_LUT4
    generic map(
      INIT => X"7F80"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_1_OBUF,
      ADR2 => pc_0_OBUF,
      ADR3 => pc_3_OBUF,
      O => XLXI_3_XLXI_1_I_Q3_TQ_pack_1
    );
  XLXI_3_XLXI_1_I_Q3_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => pc_3_OBUF_DYMUX,
      CE => pc_3_OBUF_CEINV,
      CLK => pc_3_OBUF_CLKINV,
      SET => GND,
      RST => pc_3_OBUF_FFY_RST,
      O => pc_3_OBUF
    );
  pc_3_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => pc_3_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => pc_3_OBUF_FFY_RST
    );
  pc_3_OBUF_FFY_RSTAND_137 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => rst_IBUF,
      O => pc_3_OBUF_FFY_RSTAND
    );
  XLXI_3_XLXI_1_I_Q0_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => pc_0_OBUF_DYMUX,
      CE => pc_0_OBUF_CEINV,
      CLK => pc_0_OBUF_CLKINV,
      SET => GND,
      RST => pc_0_OBUF_FFY_RST,
      O => pc_0_OBUF
    );
  pc_0_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => pc_0_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => pc_0_OBUF_FFY_RST
    );
  pc_0_OBUF_FFY_RSTAND_138 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => rst_IBUF,
      O => pc_0_OBUF_FFY_RSTAND
    );
  XLXI_3_XLXI_1_I_Q2_I_36_32 : X_LUT4
    generic map(
      INIT => X"6A6A"
    )
    port map (
      ADR0 => pc_2_OBUF,
      ADR1 => pc_1_OBUF,
      ADR2 => pc_0_OBUF,
      ADR3 => VCC,
      O => XLXI_3_XLXI_1_I_Q2_TQ_pack_1
    );
  XLXI_3_XLXI_1_I_Q2_I_36_35 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => pc_2_OBUF_DYMUX,
      CE => pc_2_OBUF_CEINV,
      CLK => pc_2_OBUF_CLKINV,
      SET => GND,
      RST => pc_2_OBUF_FFY_RST,
      O => pc_2_OBUF
    );
  pc_2_OBUF_FFY_RSTOR : X_OR2
    port map (
      I0 => pc_2_OBUF_FFY_RSTAND,
      I1 => GSR,
      O => pc_2_OBUF_FFY_RST
    );
  pc_2_OBUF_FFY_RSTAND_139 : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => rst_IBUF,
      O => pc_2_OBUF_FFY_RSTAND
    );
  XLXI_2_OV_FLG : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => OV_FLG_OUTPUT_OFF_O1INV,
      CE => OV_FLG_OUTPUT_OFF_OCEINV,
      CLK => OV_FLG_OUTPUT_OTCLK1INV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => OV_FLG_OUTPUT_OFF_OSR_USED,
      O => OV_FLG_OBUF
    );
  XLXI_2_XLXI_4_XLXI_7_I_36_8 : X_LUT4
    generic map(
      INIT => X"FC30"
    )
    port map (
      ADR0 => VCC,
      ADR1 => XLXN_31,
      ADR2 => inst_4_OBUF,
      ADR3 => R3_4_OBUF,
      O => XLXI_2_XLXN_1_4_pack_1
    );
  XLXI_2_Reg2_XLXI_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R2_4_OBUF_DXMUX,
      CE => R2_4_OBUF_CEINV,
      CLK => R2_4_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R2_4_OBUF_SRFFMUX,
      O => R2_4_OBUF
    );
  XLXN_231 : X_LUT4
    generic map(
      INIT => X"0088"
    )
    port map (
      ADR0 => inst_6_OBUF,
      ADR1 => inst_7_OBUF,
      ADR2 => VCC,
      ADR3 => inst_5_OBUF,
      O => XLXN_23
    );
  XLXI_1_XLXI_4_I_36_3 : X_LUT4
    generic map(
      INIT => X"007F"
    )
    port map (
      ADR0 => XLXI_1_XLXI_4_EQ4_5_0,
      ADR1 => XLXI_1_XLXI_4_LT0_1_0,
      ADR2 => XLXI_1_XLXI_4_EQ2_3,
      ADR3 => XLXI_1_XLXI_4_I_36_3_1_0_0,
      O => XLXN_25
    );
  XLXN_291 : X_LUT4
    generic map(
      INIT => X"0022"
    )
    port map (
      ADR0 => inst_5_OBUF,
      ADR1 => inst_7_OBUF,
      ADR2 => VCC,
      ADR3 => inst_6_OBUF,
      O => XLXN_29
    );
  XLXI_1_XLXI_55_XLXI_1_XLXI_5_I_36_8 : X_LUT4
    generic map(
      INIT => X"DC8C"
    )
    port map (
      ADR0 => inst_5_OBUF,
      ADR1 => XLXI_1_XLXI_3_MO2_0,
      ADR2 => inst_7_OBUF,
      ADR3 => XLXI_1_S(2),
      O => XLXI_1_XLXI_55_XLXN_19_2_pack_1
    );
  XLXI_1_XLXI_55_XLXI_2_XLXI_5_I_36_8 : X_LUT4
    generic map(
      INIT => X"C0AA"
    )
    port map (
      ADR0 => XLXI_1_XLXI_55_XLXN_19(2),
      ADR1 => R1_2_OBUF,
      ADR2 => R2_2_OBUF,
      ADR3 => sel_1_0,
      O => XLXN_27_2_pack_1
    );
  XLXI_2_Reg3_XLXI_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R3_2_OBUF_DXMUX,
      CE => R3_2_OBUF_CEINV,
      CLK => R3_2_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R3_2_OBUF_SRFFMUX,
      O => R3_2_OBUF
    );
  XLXI_1_XLXI_4_I_36_4 : X_LUT4
    generic map(
      INIT => X"A5A5"
    )
    port map (
      ADR0 => R2_4_OBUF,
      ADR1 => VCC,
      ADR2 => R1_4_OBUF,
      ADR3 => VCC,
      O => XLXI_1_XLXI_4_EQ4_5
    );
  XLXI_1_XLXI_3_I_MO2_I_36_8 : X_LUT4
    generic map(
      INIT => X"44F0"
    )
    port map (
      ADR0 => shf_of(0),
      ADR1 => R2_0_OBUF,
      ADR2 => XLXI_1_XLXI_3_M23,
      ADR3 => shf_of_1_0,
      O => XLXI_1_XLXI_3_MO2
    );
  XLXI_1_XLXI_55_XLXI_1_XLXI_2_I_36_8 : X_LUT4
    generic map(
      INIT => X"F044"
    )
    port map (
      ADR0 => shf_of_1_0,
      ADR1 => XLXI_1_XLXI_3_I_M45_M0_0,
      ADR2 => XLXI_1_S(0),
      ADR3 => sel_0_0,
      O => XLXI_1_XLXI_55_XLXN_19_0_pack_1
    );
  XLXI_1_XLXI_55_XLXI_2_XLXI_2_I_36_8 : X_LUT4
    generic map(
      INIT => X"D850"
    )
    port map (
      ADR0 => sel_1_0,
      ADR1 => R1_0_OBUF,
      ADR2 => XLXI_1_XLXI_55_XLXN_19(0),
      ADR3 => R2_0_OBUF,
      O => XLXN_27_0_pack_1
    );
  XLXI_1_XLXI_3_I_M12_I_36_8 : X_LUT4
    generic map(
      INIT => X"F0CC"
    )
    port map (
      ADR0 => VCC,
      ADR1 => R2_3_OBUF,
      ADR2 => R2_2_OBUF,
      ADR3 => shf_of(0),
      O => XLXI_1_XLXI_3_M12
    );
  shf_of_1_1 : X_LUT4
    generic map(
      INIT => X"2000"
    )
    port map (
      ADR0 => inst_4_OBUF,
      ADR1 => inst_6_OBUF,
      ADR2 => inst_7_OBUF,
      ADR3 => inst_5_OBUF,
      O => shf_of(1)
    );
  XLXI_1_XLXI_4_I_36_3_bound : X_LUT4
    generic map(
      INIT => X"F330"
    )
    port map (
      ADR0 => VCC,
      ADR1 => R1_4_OBUF,
      ADR2 => XLXI_1_XLXI_4_LT2_3,
      ADR3 => R2_4_OBUF,
      O => XLXI_1_XLXI_4_I_36_3_1_0
    );
  XLXI_1_XLXI_55_XLXI_2_XLXI_4_I_36_8 : X_LUT4
    generic map(
      INIT => X"B380"
    )
    port map (
      ADR0 => R2_1_OBUF,
      ADR1 => sel_1_0,
      ADR2 => R1_1_OBUF,
      ADR3 => XLXI_1_XLXI_55_XLXN_19(1),
      O => XLXN_27_1_pack_1
    );
  XLXI_2_Reg3_XLXI_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R3_1_OBUF_DXMUX,
      CE => R3_1_OBUF_CEINV,
      CLK => R3_1_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R3_1_OBUF_SRFFMUX,
      O => R3_1_OBUF
    );
  XLXN_241 : X_LUT4
    generic map(
      INIT => X"0050"
    )
    port map (
      ADR0 => inst_5_OBUF,
      ADR1 => VCC,
      ADR2 => inst_7_OBUF,
      ADR3 => inst_6_OBUF,
      O => XLXN_24
    );
  XLXI_1_XLXI_4_I_36_12 : X_LUT4
    generic map(
      INIT => X"0804"
    )
    port map (
      ADR0 => R1_3_OBUF,
      ADR1 => R1_2_OBUF,
      ADR2 => R2_2_OBUF,
      ADR3 => R2_3_OBUF,
      O => XLXI_1_XLXI_4_GE2_3
    );
  XLXN_311 : X_LUT4
    generic map(
      INIT => X"2020"
    )
    port map (
      ADR0 => inst_5_OBUF,
      ADR1 => inst_7_OBUF,
      ADR2 => inst_6_OBUF,
      ADR3 => VCC,
      O => XLXN_31_pack_1
    );
  XLXI_2_XLXI_4_XLXI_2_I_36_8 : X_LUT4
    generic map(
      INIT => X"AAF0"
    )
    port map (
      ADR0 => R3_0_OBUF,
      ADR1 => VCC,
      ADR2 => inst_0_OBUF,
      ADR3 => XLXN_31,
      O => XLXI_2_XLXN_1_0_pack_1
    );
  XLXI_2_Reg2_XLXI_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R2_0_OBUF_DXMUX,
      CE => R2_0_OBUF_CEINV,
      CLK => R2_0_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R2_0_OBUF_SRFFMUX,
      O => R2_0_OBUF
    );
  XLXI_2_GE_FLG : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => GE_FLG_OUTPUT_OFF_O1INV,
      CE => GE_FLG_OUTPUT_OFF_OCEINV,
      CLK => GE_FLG_OUTPUT_OTCLK1INV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => GE_FLG_OUTPUT_OFF_OSR_USED,
      O => GE_FLG_OBUF
    );
  XLXI_2_Reg3_XLXI_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R3_0_OBUF_DXMUX,
      CE => R3_0_OBUF_CEINV,
      CLK => R3_0_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R3_0_OBUF_SRFFMUX,
      O => R3_0_OBUF
    );
  XLXI_1_XLXI_55_XLXI_1_XLXI_6_I_36_8 : X_LUT4
    generic map(
      INIT => X"FD20"
    )
    port map (
      ADR0 => inst_7_OBUF,
      ADR1 => inst_5_OBUF,
      ADR2 => XLXI_1_S(3),
      ADR3 => XLXI_1_XLXI_3_MO1_0,
      O => XLXI_1_XLXI_55_XLXN_19_3_pack_1
    );
  XLXI_1_XLXI_55_XLXI_2_XLXI_6_I_36_8 : X_LUT4
    generic map(
      INIT => X"B830"
    )
    port map (
      ADR0 => R1_3_OBUF,
      ADR1 => sel_1_0,
      ADR2 => XLXI_1_XLXI_55_XLXN_19(3),
      ADR3 => R2_3_OBUF,
      O => XLXN_27_3_pack_1
    );
  XLXI_2_Reg3_XLXI_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R3_3_OBUF_DXMUX,
      CE => R3_3_OBUF_CEINV,
      CLK => R3_3_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R3_3_OBUF_SRFFMUX,
      O => R3_3_OBUF
    );
  XLXN_321 : X_LUT4
    generic map(
      INIT => X"D0D0"
    )
    port map (
      ADR0 => inst_6_OBUF,
      ADR1 => inst_5_OBUF,
      ADR2 => inst_7_OBUF,
      ADR3 => VCC,
      O => XLXN_32
    );
  XLXN_301 : X_LUT4
    generic map(
      INIT => X"3300"
    )
    port map (
      ADR0 => VCC,
      ADR1 => inst_7_OBUF,
      ADR2 => VCC,
      ADR3 => inst_6_OBUF,
      O => XLXN_30
    );
  XLXI_1_XLXI_3_I_MO1_I_36_8 : X_LUT4
    generic map(
      INIT => X"CFC0"
    )
    port map (
      ADR0 => VCC,
      ADR1 => XLXI_1_XLXI_3_M34,
      ADR2 => shf_of_1_0,
      ADR3 => XLXI_1_XLXI_3_M12_0,
      O => XLXI_1_XLXI_3_MO1
    );
  XLXI_1_XLXI_55_XLXI_1_XLXI_7_I_36_8 : X_LUT4
    generic map(
      INIT => X"AEA2"
    )
    port map (
      ADR0 => XLXI_1_XLXI_3_MO0_0,
      ADR1 => inst_7_OBUF,
      ADR2 => inst_5_OBUF,
      ADR3 => XLXI_1_S(4),
      O => XLXI_1_XLXI_55_XLXN_19_4_pack_1
    );
  XLXI_1_XLXI_55_XLXI_2_XLXI_7_I_36_8 : X_LUT4
    generic map(
      INIT => X"D580"
    )
    port map (
      ADR0 => sel_1_0,
      ADR1 => R1_4_OBUF,
      ADR2 => R2_4_OBUF,
      ADR3 => XLXI_1_XLXI_55_XLXN_19(4),
      O => XLXN_27_4_pack_1
    );
  XLXI_2_Reg3_XLXI_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => R3_4_OBUF_DXMUX,
      CE => R3_4_OBUF_CEINV,
      CLK => R3_4_OBUF_CLKINV,
      SET => GND,
      RST => GSR,
      SSET => GND,
      SRST => R3_4_OBUF_SRFFMUX,
      O => R3_4_OBUF
    );
  XLXI_1_XLXI_3_I_MO0_I_36_8 : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => XLXI_1_XLXI_3_M01_0,
      ADR1 => shf_of_1_0,
      ADR2 => VCC,
      ADR3 => XLXI_1_XLXI_3_M23,
      O => XLXI_1_XLXI_3_MO0
    );
  GLOBAL_LOGIC0_GND : X_ZERO
    port map (
      O => GLOBAL_LOGIC0
    );
  GLOBAL_LOGIC1_VCC : X_ONE
    port map (
      O => GLOBAL_LOGIC1
    );
  XLXI_1_S_4_G_X_LUT4 : X_LUT4
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_S_4_G
    );
  XLXI_1_XLXI_2_C6O_F_X_LUT4 : X_LUT4
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_XLXI_2_C6O_F
    );
  XLXI_1_XLXI_2_C6O_G_X_LUT4 : X_LUT4
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => VCC,
      ADR1 => VCC,
      ADR2 => VCC,
      ADR3 => VCC,
      O => XLXI_1_XLXI_2_C6O_G
    );
  R3_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_2_OBUF,
      O => R3_2_O
    );
  R3_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_3_OBUF,
      O => R3_3_O
    );
  R3_4_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_4_OBUF,
      O => R3_4_O
    );
  pc_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_0_OBUF,
      O => pc_0_O
    );
  R2_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_3_OBUF,
      O => R2_3_O
    );
  R3_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_0_OBUF,
      O => R3_0_O
    );
  R2_4_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_4_OBUF,
      O => R2_4_O
    );
  R3_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R3_1_OBUF,
      O => R3_1_O
    );
  pc_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_1_OBUF,
      O => pc_1_O
    );
  pc_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_2_OBUF,
      O => pc_2_O
    );
  pc_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => pc_3_OBUF,
      O => pc_3_O
    );
  inst_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_0_OBUF,
      O => inst_0_O
    );
  inst_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_1_OBUF,
      O => inst_1_O
    );
  inst_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_2_OBUF,
      O => inst_2_O
    );
  inst_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_3_OBUF,
      O => inst_3_O
    );
  inst_4_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_4_OBUF,
      O => inst_4_O
    );
  R1_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_0_OBUF,
      O => R1_0_O
    );
  inst_5_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_5_OBUF,
      O => inst_5_O
    );
  R1_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_1_OBUF,
      O => R1_1_O
    );
  inst_6_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_6_OBUF,
      O => inst_6_O
    );
  R1_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_2_OBUF,
      O => R1_2_O
    );
  inst_7_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => inst_7_OBUF,
      O => inst_7_O
    );
  R1_3_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_3_OBUF,
      O => R1_3_O
    );
  R2_0_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_0_OBUF,
      O => R2_0_O
    );
  R1_4_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R1_4_OBUF,
      O => R1_4_O
    );
  R2_1_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_1_OBUF,
      O => R2_1_O
    );
  R2_2_OUTPUT_OFF_OMUX : X_BUF_PP
    generic map(
      PATHPULSE => 665 ps
    )
    port map (
      I => R2_2_OBUF,
      O => R2_2_O
    );
  NlwBlock_machine_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_machine_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

