--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : Mask5.vhf
-- /___/   /\     Timestamp : 05/31/2005 23:48:58
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w Mask5.sch Mask5.vhf
--Design Name: Mask5
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

entity Mask5 is
   port ( A : in    std_logic_vector (4 downto 0); 
          B : in    std_logic_vector (4 downto 0); 
          C : out   std_logic_vector (4 downto 0));
end Mask5;

architecture BEHAVIORAL of Mask5 is
   attribute BOX_TYPE   : string ;
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
begin
   XLXI_3 : AND2
      port map (I0=>B(0),
                I1=>A(0),
                O=>C(0));
   
   XLXI_4 : AND2
      port map (I0=>B(1),
                I1=>A(1),
                O=>C(1));
   
   XLXI_5 : AND2
      port map (I0=>B(2),
                I1=>A(2),
                O=>C(2));
   
   XLXI_6 : AND2
      port map (I0=>B(3),
                I1=>A(3),
                O=>C(3));
   
   XLXI_7 : AND2
      port map (I0=>B(4),
                I1=>A(4),
                O=>C(4));
   
end BEHAVIORAL;


