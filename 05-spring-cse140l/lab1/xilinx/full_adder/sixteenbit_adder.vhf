--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : sixteenbit_adder.vhf
-- /___/   /\     Timestamp : 04/18/2005 21:10:40
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w sixteenbit_adder.sch sixteenbit_adder.vhf
--Design Name: sixteenbit_adder
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

entity sixteenbit_adder is
   port ( A  : in    std_logic_vector (15 downto 0); 
          B  : in    std_logic_vector (15 downto 0); 
          Ci : in    std_logic; 
          Co : out   std_logic; 
          S  : out   std_logic_vector (15 downto 0));
end sixteenbit_adder;

architecture BEHAVIORAL of sixteenbit_adder is
   signal XLXN_1 : std_logic;
   signal XLXN_2 : std_logic;
   signal XLXN_3 : std_logic;
   component fourbit_adder
      port ( A  : in    std_logic_vector (3 downto 0); 
             B  : in    std_logic_vector (3 downto 0); 
             Ci : in    std_logic; 
             Co : out   std_logic; 
             S  : out   std_logic_vector (3 downto 0));
   end component;
   
begin
   XLXI_2 : fourbit_adder
      port map (A(3 downto 0)=>A(3 downto 0),
                B(3 downto 0)=>B(3 downto 0),
                Ci=>Ci,
                Co=>XLXN_1,
                S(3 downto 0)=>S(3 downto 0));
   
   XLXI_3 : fourbit_adder
      port map (A(3 downto 0)=>A(7 downto 4),
                B(3 downto 0)=>B(7 downto 4),
                Ci=>XLXN_1,
                Co=>XLXN_2,
                S(3 downto 0)=>S(7 downto 4));
   
   XLXI_4 : fourbit_adder
      port map (A(3 downto 0)=>A(11 downto 8),
                B(3 downto 0)=>B(11 downto 8),
                Ci=>XLXN_2,
                Co=>XLXN_3,
                S(3 downto 0)=>S(11 downto 8));
   
   XLXI_5 : fourbit_adder
      port map (A(3 downto 0)=>A(15 downto 12),
                B(3 downto 0)=>B(15 downto 12),
                Ci=>XLXN_3,
                Co=>Co,
                S(3 downto 0)=>S(15 downto 12));
   
end BEHAVIORAL;


