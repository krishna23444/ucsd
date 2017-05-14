--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : asyncounter_8.vhf
-- /___/   /\     Timestamp : 05/04/2005 00:12:34
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w asyncounter_8.sch asyncounter_8.vhf
--Design Name: asyncounter_8
--Device: virtex2
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesis and simulted, but it should not be modified. 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity FTCE_MXILINX_asyncounter_8 is
   port ( C   : in    std_logic; 
          CE  : in    std_logic; 
          CLR : in    std_logic; 
          T   : in    std_logic; 
          Q   : out   std_logic);
end FTCE_MXILINX_asyncounter_8;

architecture BEHAVIORAL of FTCE_MXILINX_asyncounter_8 is
   attribute BOX_TYPE   : string ;
   attribute INIT       : string ;
   attribute RLOC       : string ;
   signal TQ      : std_logic;
   signal Q_DUMMY : std_logic;
   component XOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XOR2 : component is "BLACK_BOX";
   
   component FDCE
      -- synopsys translate_off
      generic( INIT : bit :=  '0');
      -- synopsys translate_on
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute INIT of FDCE : component is "0";
   attribute BOX_TYPE of FDCE : component is "BLACK_BOX";
   
   attribute RLOC of I_36_35 : label is "X0Y0";
begin
   Q <= Q_DUMMY;
   I_36_32 : XOR2
      port map (I0=>T,
                I1=>Q_DUMMY,
                O=>TQ);
   
   I_36_35 : FDCE
      port map (C=>C,
                CE=>CE,
                CLR=>CLR,
                D=>TQ,
                Q=>Q_DUMMY);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity asyncounter_8 is
   port ( CLK    : in    std_logic; 
          CLK_CE : in    std_logic; 
          CLR    : in    std_logic; 
          VCC    : in    std_logic; 
          D      : out   std_logic_vector (7 downto 0));
end asyncounter_8;

architecture BEHAVIORAL of asyncounter_8 is
   attribute HU_SET     : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_22 : std_logic;
   signal XLXN_24 : std_logic;
   signal XLXN_31 : std_logic;
   signal XLXN_37 : std_logic;
   signal XLXN_40 : std_logic;
   signal XLXN_43 : std_logic;
   signal XLXN_47 : std_logic;
   signal D_DUMMY : std_logic_vector (7 downto 0);
   component FTCE_MXILINX_asyncounter_8
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             T   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   attribute HU_SET of XLXI_2 : label is "XLXI_2_0";
   attribute HU_SET of XLXI_3 : label is "XLXI_3_1";
   attribute HU_SET of XLXI_4 : label is "XLXI_4_2";
   attribute HU_SET of XLXI_5 : label is "XLXI_5_3";
   attribute HU_SET of XLXI_10 : label is "XLXI_10_4";
   attribute HU_SET of XLXI_11 : label is "XLXI_11_5";
   attribute HU_SET of XLXI_12 : label is "XLXI_12_6";
   attribute HU_SET of XLXI_13 : label is "XLXI_13_7";
begin
   D(7 downto 0) <= D_DUMMY(7 downto 0);
   XLXI_2 : FTCE_MXILINX_asyncounter_8
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(0));
   
   XLXI_3 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_31,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(1));
   
   XLXI_4 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_22,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(2));
   
   XLXI_5 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_24,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(3));
   
   XLXI_7 : INV
      port map (I=>D_DUMMY(1),
                O=>XLXN_22);
   
   XLXI_8 : INV
      port map (I=>D_DUMMY(2),
                O=>XLXN_24);
   
   XLXI_9 : INV
      port map (I=>D_DUMMY(0),
                O=>XLXN_31);
   
   XLXI_10 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_47,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(4));
   
   XLXI_11 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_40,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(7));
   
   XLXI_12 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_43,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(5));
   
   XLXI_13 : FTCE_MXILINX_asyncounter_8
      port map (C=>XLXN_37,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D_DUMMY(6));
   
   XLXI_14 : INV
      port map (I=>D_DUMMY(5),
                O=>XLXN_37);
   
   XLXI_15 : INV
      port map (I=>D_DUMMY(6),
                O=>XLXN_40);
   
   XLXI_16 : INV
      port map (I=>D_DUMMY(4),
                O=>XLXN_43);
   
   XLXI_17 : INV
      port map (I=>D_DUMMY(3),
                O=>XLXN_47);
   
end BEHAVIORAL;


