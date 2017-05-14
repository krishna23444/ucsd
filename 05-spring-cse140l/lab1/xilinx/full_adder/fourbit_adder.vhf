--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : fourbit_adder.vhf
-- /___/   /\     Timestamp : 04/18/2005 20:12:35
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w fourbit_adder.sch fourbit_adder.vhf
--Design Name: fourbit_adder
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

entity fourbit_adder is
   port ( A  : in    std_logic_vector (3 downto 0); 
          B  : in    std_logic_vector (3 downto 0); 
          Ci : in    std_logic; 
          Co : out   std_logic; 
          S  : out   std_logic_vector (3 downto 0));
end fourbit_adder;

architecture BEHAVIORAL of fourbit_adder is
   signal XLXN_3  : std_logic;
   signal XLXN_7  : std_logic;
   signal XLXN_10 : std_logic;
   component full_adder
      port ( A  : in    std_logic; 
             B  : in    std_logic; 
             Ci : in    std_logic; 
             Co : out   std_logic; 
             S  : out   std_logic);
   end component;
   
begin
   XLXI_1 : full_adder
      port map (A=>A(0),
                B=>B(0),
                Ci=>Ci,
                Co=>XLXN_3,
                S=>S(0));
   
   XLXI_2 : full_adder
      port map (A=>A(1),
                B=>B(1),
                Ci=>XLXN_3,
                Co=>XLXN_7,
                S=>S(1));
   
   XLXI_3 : full_adder
      port map (A=>A(2),
                B=>B(2),
                Ci=>XLXN_7,
                Co=>XLXN_10,
                S=>S(2));
   
   XLXI_4 : full_adder
      port map (A=>A(3),
                B=>B(3),
                Ci=>XLXN_10,
                Co=>Co,
                S=>S(3));
   
end BEHAVIORAL;


