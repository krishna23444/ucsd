--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : Mux5_3.vhf
-- /___/   /\     Timestamp : 05/31/2005 20:55:44
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w Mux5_3.sch Mux5_3.vhf
--Design Name: Mux5_3
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

entity M2_1_MXILINX_Mux5_3 is
   port ( D0 : in    std_logic; 
          D1 : in    std_logic; 
          S0 : in    std_logic; 
          O  : out   std_logic);
end M2_1_MXILINX_Mux5_3;

architecture BEHAVIORAL of M2_1_MXILINX_Mux5_3 is
   attribute BOX_TYPE   : string ;
   signal M0 : std_logic;
   signal M1 : std_logic;
   component AND2B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2B1 : component is "BLACK_BOX";
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
begin
   I_36_7 : AND2B1
      port map (I0=>S0,
                I1=>D0,
                O=>M0);
   
   I_36_8 : OR2
      port map (I0=>M1,
                I1=>M0,
                O=>O);
   
   I_36_9 : AND2
      port map (I0=>D1,
                I1=>S0,
                O=>M1);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity Mux5_3 is
   port ( A : in    std_logic_vector (4 downto 0); 
          B : in    std_logic_vector (4 downto 0); 
          S : in    std_logic; 
          C : out   std_logic_vector (4 downto 0));
end Mux5_3;

architecture BEHAVIORAL of Mux5_3 is
   attribute HU_SET     : string ;
   component M2_1_MXILINX_Mux5_3
      port ( D0 : in    std_logic; 
             D1 : in    std_logic; 
             S0 : in    std_logic; 
             O  : out   std_logic);
   end component;
   
   attribute HU_SET of XLXI_2 : label is "XLXI_2_0";
   attribute HU_SET of XLXI_4 : label is "XLXI_4_1";
   attribute HU_SET of XLXI_5 : label is "XLXI_5_2";
   attribute HU_SET of XLXI_6 : label is "XLXI_6_3";
   attribute HU_SET of XLXI_7 : label is "XLXI_7_4";
begin
   XLXI_2 : M2_1_MXILINX_Mux5_3
      port map (D0=>A(0),
                D1=>B(0),
                S0=>S,
                O=>C(0));
   
   XLXI_4 : M2_1_MXILINX_Mux5_3
      port map (D0=>A(1),
                D1=>B(1),
                S0=>S,
                O=>C(1));
   
   XLXI_5 : M2_1_MXILINX_Mux5_3
      port map (D0=>A(2),
                D1=>B(2),
                S0=>S,
                O=>C(2));
   
   XLXI_6 : M2_1_MXILINX_Mux5_3
      port map (D0=>A(3),
                D1=>B(3),
                S0=>S,
                O=>C(3));
   
   XLXI_7 : M2_1_MXILINX_Mux5_3
      port map (D0=>A(4),
                D1=>B(4),
                S0=>S,
                O=>C(4));
   
end BEHAVIORAL;


