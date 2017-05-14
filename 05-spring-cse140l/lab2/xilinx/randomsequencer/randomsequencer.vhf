--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : randomsequencer.vhf
-- /___/   /\     Timestamp : 05/03/2005 23:13:37
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w randomsequencer.sch randomsequencer.vhf
--Design Name: randomsequencer
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

entity randomsequencer is
   port ( CLK    : in    std_logic; 
          CLK_CE : in    std_logic; 
          PRE    : in    std_logic; 
          D      : out   std_logic_vector (3 downto 0));
end randomsequencer;

architecture BEHAVIORAL of randomsequencer is
   attribute INIT       : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_8  : std_logic;
   signal D_DUMMY : std_logic_vector (3 downto 0);
   component FDPE
      -- synopsys translate_off
      generic( INIT : bit :=  '1');
      -- synopsys translate_on
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             D   : in    std_logic; 
             PRE : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute INIT of FDPE : component is "1";
   attribute BOX_TYPE of FDPE : component is "BLACK_BOX";
   
   component XOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XOR2 : component is "BLACK_BOX";
   
begin
   D(3 downto 0) <= D_DUMMY(3 downto 0);
   XLXI_2 : FDPE
      port map (C=>CLK,
                CE=>CLK_CE,
                D=>XLXN_8,
                PRE=>PRE,
                Q=>D_DUMMY(0));
   
   XLXI_3 : FDPE
      port map (C=>CLK,
                CE=>CLK_CE,
                D=>D_DUMMY(0),
                PRE=>PRE,
                Q=>D_DUMMY(1));
   
   XLXI_4 : FDPE
      port map (C=>CLK,
                CE=>CLK_CE,
                D=>D_DUMMY(1),
                PRE=>PRE,
                Q=>D_DUMMY(2));
   
   XLXI_5 : FDPE
      port map (C=>CLK,
                CE=>CLK_CE,
                D=>D_DUMMY(2),
                PRE=>PRE,
                Q=>D_DUMMY(3));
   
   XLXI_9 : XOR2
      port map (I0=>D_DUMMY(3),
                I1=>D_DUMMY(2),
                O=>XLXN_8);
   
end BEHAVIORAL;


