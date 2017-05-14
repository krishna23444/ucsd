--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : Mux3_5bus.vhf
-- /___/   /\     Timestamp : 05/31/2005 23:48:59
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w Mux3_5bus.sch Mux3_5bus.vhf
--Design Name: Mux3_5bus
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

entity M2_1E_MXILINX_Mux3_5bus is
   port ( D0 : in    std_logic; 
          D1 : in    std_logic; 
          E  : in    std_logic; 
          S0 : in    std_logic; 
          O  : out   std_logic);
end M2_1E_MXILINX_Mux3_5bus;

architecture BEHAVIORAL of M2_1E_MXILINX_Mux3_5bus is
   attribute BOX_TYPE   : string ;
   signal M0 : std_logic;
   signal M1 : std_logic;
   component AND3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3 : component is "BLACK_BOX";
   
   component AND3B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B1 : component is "BLACK_BOX";
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
begin
   I_36_30 : AND3
      port map (I0=>D1,
                I1=>E,
                I2=>S0,
                O=>M1);
   
   I_36_31 : AND3B1
      port map (I0=>S0,
                I1=>E,
                I2=>D0,
                O=>M0);
   
   I_36_38 : OR2
      port map (I0=>M1,
                I1=>M0,
                O=>O);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity M4_1E_MXILINX_Mux3_5bus is
   port ( D0 : in    std_logic; 
          D1 : in    std_logic; 
          D2 : in    std_logic; 
          D3 : in    std_logic; 
          E  : in    std_logic; 
          S0 : in    std_logic; 
          S1 : in    std_logic; 
          O  : out   std_logic);
end M4_1E_MXILINX_Mux3_5bus;

architecture BEHAVIORAL of M4_1E_MXILINX_Mux3_5bus is
   attribute HU_SET     : string ;
   attribute BOX_TYPE   : string ;
   signal M01 : std_logic;
   signal M23 : std_logic;
   component M2_1E_MXILINX_Mux3_5bus
      port ( D0 : in    std_logic; 
             D1 : in    std_logic; 
             E  : in    std_logic; 
             S0 : in    std_logic; 
             O  : out   std_logic);
   end component;
   
   component MUXF5
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             S  : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of MUXF5 : component is "BLACK_BOX";
   
   attribute HU_SET of I_M01 : label is "I_M01_1";
   attribute HU_SET of I_M23 : label is "I_M23_0";
begin
   I_M01 : M2_1E_MXILINX_Mux3_5bus
      port map (D0=>D0,
                D1=>D1,
                E=>E,
                S0=>S0,
                O=>M01);
   
   I_M23 : M2_1E_MXILINX_Mux3_5bus
      port map (D0=>D2,
                D1=>D3,
                E=>E,
                S0=>S0,
                O=>M23);
   
   I_O : MUXF5
      port map (I0=>M01,
                I1=>M23,
                S=>S1,
                O=>O);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity Mux3_5bus is
   port ( A : in    std_logic_vector (4 downto 0); 
          B : in    std_logic_vector (4 downto 0); 
          C : in    std_logic_vector (4 downto 0); 
          S : in    std_logic_vector (1 downto 0); 
          O : out   std_logic_vector (4 downto 0));
end Mux3_5bus;

architecture BEHAVIORAL of Mux3_5bus is
   attribute HU_SET     : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_30  : std_logic;
   signal XLXN_51  : std_logic;
   signal XLXN_155 : std_logic;
   signal XLXN_162 : std_logic;
   signal XLXN_169 : std_logic;
   component M4_1E_MXILINX_Mux3_5bus
      port ( D0 : in    std_logic; 
             D1 : in    std_logic; 
             D2 : in    std_logic; 
             D3 : in    std_logic; 
             E  : in    std_logic; 
             S0 : in    std_logic; 
             S1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   attribute HU_SET of XLXI_9 : label is "XLXI_9_2";
   attribute HU_SET of XLXI_14 : label is "XLXI_14_3";
   attribute HU_SET of XLXI_38 : label is "XLXI_38_4";
   attribute HU_SET of XLXI_40 : label is "XLXI_40_5";
   attribute HU_SET of XLXI_42 : label is "XLXI_42_6";
begin
   XLXI_9 : M4_1E_MXILINX_Mux3_5bus
      port map (D0=>A(0),
                D1=>B(0),
                D2=>C(0),
                D3=>XLXN_30,
                E=>XLXN_30,
                S0=>S(0),
                S1=>S(1),
                O=>O(0));
   
   XLXI_10 : GND
      port map (G=>XLXN_30);
   
   XLXI_14 : M4_1E_MXILINX_Mux3_5bus
      port map (D0=>A(1),
                D1=>B(1),
                D2=>C(1),
                D3=>XLXN_51,
                E=>XLXN_51,
                S0=>S(0),
                S1=>S(1),
                O=>O(1));
   
   XLXI_15 : GND
      port map (G=>XLXN_51);
   
   XLXI_38 : M4_1E_MXILINX_Mux3_5bus
      port map (D0=>A(2),
                D1=>B(2),
                D2=>C(2),
                D3=>XLXN_155,
                E=>XLXN_155,
                S0=>S(0),
                S1=>S(1),
                O=>O(2));
   
   XLXI_39 : GND
      port map (G=>XLXN_155);
   
   XLXI_40 : M4_1E_MXILINX_Mux3_5bus
      port map (D0=>A(3),
                D1=>B(3),
                D2=>C(3),
                D3=>XLXN_162,
                E=>XLXN_162,
                S0=>S(0),
                S1=>S(1),
                O=>O(3));
   
   XLXI_41 : GND
      port map (G=>XLXN_162);
   
   XLXI_42 : M4_1E_MXILINX_Mux3_5bus
      port map (D0=>A(4),
                D1=>B(4),
                D2=>C(4),
                D3=>XLXN_169,
                E=>XLXN_169,
                S0=>S(0),
                S1=>S(1),
                O=>O(4));
   
   XLXI_43 : GND
      port map (G=>XLXN_169);
   
end BEHAVIORAL;


