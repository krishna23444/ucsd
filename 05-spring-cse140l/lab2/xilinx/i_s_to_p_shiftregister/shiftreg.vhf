--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : shiftreg.vhf
-- /___/   /\     Timestamp : 05/03/2005 01:31:38
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w shiftreg.sch shiftreg.vhf
--Design Name: shiftreg
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

entity shiftreg is
   port ( CLK    : in    std_logic; 
          CLK_EN : in    std_logic; 
          CLR    : in    std_logic; 
          Din    : in    std_logic; 
          D0     : out   std_logic; 
          D1     : out   std_logic; 
          D2     : out   std_logic; 
          D3     : out   std_logic);
end shiftreg;

architecture BEHAVIORAL of shiftreg is
   attribute INIT       : string ;
   attribute BOX_TYPE   : string ;
   signal D0_DUMMY : std_logic;
   signal D1_DUMMY : std_logic;
   signal D2_DUMMY : std_logic;
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
   
begin
   D0 <= D0_DUMMY;
   D1 <= D1_DUMMY;
   D2 <= D2_DUMMY;
   XLXI_1 : FDCE
      port map (C=>CLK,
                CE=>CLK_EN,
                CLR=>CLR,
                D=>Din,
                Q=>D0_DUMMY);
   
   XLXI_3 : FDCE
      port map (C=>CLK,
                CE=>CLK_EN,
                CLR=>CLR,
                D=>D0_DUMMY,
                Q=>D1_DUMMY);
   
   XLXI_4 : FDCE
      port map (C=>CLK,
                CE=>CLK_EN,
                CLR=>CLR,
                D=>D1_DUMMY,
                Q=>D2_DUMMY);
   
   XLXI_5 : FDCE
      port map (C=>CLK,
                CE=>CLK_EN,
                CLR=>CLR,
                D=>D2_DUMMY,
                Q=>D3);
   
end BEHAVIORAL;


