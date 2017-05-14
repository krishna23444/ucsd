--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : asyncounter.vhf
-- /___/   /\     Timestamp : 05/03/2005 02:49:37
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w asyncounter.sch asyncounter.vhf
--Design Name: asyncounter
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

entity FTCE_MXILINX_asyncounter is
   port ( C   : in    std_logic; 
          CE  : in    std_logic; 
          CLR : in    std_logic; 
          T   : in    std_logic; 
          Q   : out   std_logic);
end FTCE_MXILINX_asyncounter;

architecture BEHAVIORAL of FTCE_MXILINX_asyncounter is
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

entity asyncounter is
   port ( CLK    : in    std_logic; 
          CLK_CE : in    std_logic; 
          CLR    : in    std_logic; 
          VCC    : in    std_logic; 
          D0     : out   std_logic; 
          D1     : out   std_logic; 
          D2     : out   std_logic; 
          D3     : out   std_logic);
end asyncounter;

architecture BEHAVIORAL of asyncounter is
   attribute HU_SET     : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_22  : std_logic;
   signal XLXN_24  : std_logic;
   signal XLXN_31  : std_logic;
   signal D0_DUMMY : std_logic;
   signal D1_DUMMY : std_logic;
   signal D2_DUMMY : std_logic;
   component FTCE_MXILINX_asyncounter
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
begin
   D0 <= D0_DUMMY;
   D1 <= D1_DUMMY;
   D2 <= D2_DUMMY;
   XLXI_2 : FTCE_MXILINX_asyncounter
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D0_DUMMY);
   
   XLXI_3 : FTCE_MXILINX_asyncounter
      port map (C=>XLXN_31,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D1_DUMMY);
   
   XLXI_4 : FTCE_MXILINX_asyncounter
      port map (C=>XLXN_22,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D2_DUMMY);
   
   XLXI_5 : FTCE_MXILINX_asyncounter
      port map (C=>XLXN_24,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>VCC,
                Q=>D3);
   
   XLXI_7 : INV
      port map (I=>D1_DUMMY,
                O=>XLXN_22);
   
   XLXI_8 : INV
      port map (I=>D2_DUMMY,
                O=>XLXN_24);
   
   XLXI_9 : INV
      port map (I=>D0_DUMMY,
                O=>XLXN_31);
   
end BEHAVIORAL;


