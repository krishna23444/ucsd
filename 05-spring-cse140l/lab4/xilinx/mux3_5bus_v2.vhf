--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : Mux3_5bus_v2.vhf
-- /___/   /\     Timestamp : 06/01/2005 01:43:59
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w Mux3_5bus_v2.sch Mux3_5bus_v2.vhf
--Design Name: Mux3_5bus_v2
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

entity Mux3_5bus_v2 is
   port ( A   : in    std_logic_vector (4 downto 0); 
          B   : in    std_logic_vector (4 downto 0); 
          C   : in    std_logic_vector (4 downto 0); 
          sel : in    std_logic_vector (1 downto 0); 
          O   : out   std_logic_vector (4 downto 0));
end Mux3_5bus_v2;

architecture BEHAVIORAL of Mux3_5bus_v2 is
   signal XLXN_19 : std_logic_vector (4 downto 0);
   component Mux2_5bus
      port ( A : in    std_logic_vector (4 downto 0); 
             B : in    std_logic_vector (4 downto 0); 
             C : out   std_logic_vector (4 downto 0); 
             S : in    std_logic);
   end component;
   
begin
   XLXI_1 : Mux2_5bus
      port map (A(4 downto 0)=>A(4 downto 0),
                B(4 downto 0)=>B(4 downto 0),
                S=>sel(0),
                C(4 downto 0)=>XLXN_19(4 downto 0));
   
   XLXI_2 : Mux2_5bus
      port map (A(4 downto 0)=>XLXN_19(4 downto 0),
                B(4 downto 0)=>C(4 downto 0),
                S=>sel(1),
                C(4 downto 0)=>O(4 downto 0));
   
end BEHAVIORAL;


