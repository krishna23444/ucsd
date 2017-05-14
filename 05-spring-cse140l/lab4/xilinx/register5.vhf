--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : register5.vhf
-- /___/   /\     Timestamp : 05/31/2005 23:49:00
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w register5.sch register5.vhf
--Design Name: register5
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

entity register5 is
   port ( clk    : in    std_logic; 
          clk_en : in    std_logic; 
          clr    : in    std_logic; 
          D      : in    std_logic_vector (4 downto 0); 
          Q      : out   std_logic_vector (4 downto 0));
end register5;

architecture BEHAVIORAL of register5 is
   attribute INIT       : string ;
   attribute BOX_TYPE   : string ;
   component FDRE
      -- synopsys translate_off
      generic( INIT : bit :=  '0');
      -- synopsys translate_on
      port ( C  : in    std_logic; 
             CE : in    std_logic; 
             D  : in    std_logic; 
             R  : in    std_logic; 
             Q  : out   std_logic);
   end component;
   attribute INIT of FDRE : component is "0";
   attribute BOX_TYPE of FDRE : component is "BLACK_BOX";
   
begin
   XLXI_1 : FDRE
      port map (C=>clk,
                CE=>clk_en,
                D=>D(1),
                R=>clr,
                Q=>Q(1));
   
   XLXI_2 : FDRE
      port map (C=>clk,
                CE=>clk_en,
                D=>D(0),
                R=>clr,
                Q=>Q(0));
   
   XLXI_3 : FDRE
      port map (C=>clk,
                CE=>clk_en,
                D=>D(2),
                R=>clr,
                Q=>Q(2));
   
   XLXI_4 : FDRE
      port map (C=>clk,
                CE=>clk_en,
                D=>D(3),
                R=>clr,
                Q=>Q(3));
   
   XLXI_5 : FDRE
      port map (C=>clk,
                CE=>clk_en,
                D=>D(4),
                R=>clr,
                Q=>Q(4));
   
end BEHAVIORAL;


