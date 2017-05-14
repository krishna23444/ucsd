--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : memory16x8.vhf
-- /___/   /\     Timestamp : 06/01/2005 14:06:47
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w memory16x8.sch memory16x8.vhf
--Design Name: memory16x8
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

entity memory16x8 is
   port ( addr : in    std_logic_vector (3 downto 0); 
          data : out   std_logic_vector (7 downto 0));
end memory16x8;

architecture BEHAVIORAL of memory16x8 is
   attribute INIT       : string ;
   attribute BOX_TYPE   : string ;
   component ROM16X1
      -- synopsys translate_off
      generic( INIT : bit_vector :=  x"0000");
      -- synopsys translate_on
      port ( A0 : in    std_logic; 
             A1 : in    std_logic; 
             A2 : in    std_logic; 
             A3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute INIT of ROM16X1 : component is "0000";
   attribute BOX_TYPE of ROM16X1 : component is "BLACK_BOX";
   
   attribute INIT of XLXI_1 : label is "1948";
   attribute INIT of XLXI_9 : label is "1A94";
   attribute INIT of XLXI_14 : label is "17B2";
   attribute INIT of XLXI_15 : label is "0102";
   attribute INIT of XLXI_16 : label is "0422";
   attribute INIT of XLXI_18 : label is "0404";
   attribute INIT of XLXI_19 : label is "0406";
begin
   XLXI_1 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"1948")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(7));
   
   XLXI_9 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"1A94")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(6));
   
   XLXI_14 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"17B2")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(5));
   
   XLXI_15 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"0102")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(4));
   
   XLXI_16 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"0422")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(0));
   
   XLXI_18 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"0404")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(1));
   
   XLXI_19 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"0406")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(2));
   
   XLXI_20 : ROM16X1
   -- synopsys translate_off
   generic map( INIT => x"0000")
   -- synopsys translate_on
      port map (A0=>addr(0),
                A1=>addr(1),
                A2=>addr(2),
                A3=>addr(3),
                O=>data(3));
   
end BEHAVIORAL;


