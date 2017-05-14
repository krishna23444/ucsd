--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : datapath.vhf
-- /___/   /\     Timestamp : 06/01/2005 01:40:48
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w datapath.sch datapath.vhf
--Design Name: datapath
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

entity ADD8_MXILINX_datapath is
   port ( A   : in    std_logic_vector (7 downto 0); 
          B   : in    std_logic_vector (7 downto 0); 
          CI  : in    std_logic; 
          CO  : out   std_logic; 
          OFL : out   std_logic; 
          S   : out   std_logic_vector (7 downto 0));
end ADD8_MXILINX_datapath;

architecture BEHAVIORAL of ADD8_MXILINX_datapath is
   attribute BOX_TYPE   : string ;
   attribute RLOC       : string ;
   signal C0       : std_logic;
   signal C1       : std_logic;
   signal C2       : std_logic;
   signal C3       : std_logic;
   signal C4       : std_logic;
   signal C5       : std_logic;
   signal C6       : std_logic;
   signal C6O      : std_logic;
   signal dummy    : std_logic;
   signal I0       : std_logic;
   signal I1       : std_logic;
   signal I2       : std_logic;
   signal I3       : std_logic;
   signal I4       : std_logic;
   signal I5       : std_logic;
   signal I6       : std_logic;
   signal I7       : std_logic;
   signal CO_DUMMY : std_logic;
   component FMAP
      port ( I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             I4 : in    std_logic; 
             O  : in    std_logic);
   end component;
   attribute BOX_TYPE of FMAP : component is "BLACK_BOX";
   
   component MUXCY_L
      port ( CI : in    std_logic; 
             DI : in    std_logic; 
             S  : in    std_logic; 
             LO : out   std_logic);
   end component;
   attribute BOX_TYPE of MUXCY_L : component is "BLACK_BOX";
   
   component MUXCY
      port ( CI : in    std_logic; 
             DI : in    std_logic; 
             S  : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of MUXCY : component is "BLACK_BOX";
   
   component XORCY
      port ( CI : in    std_logic; 
             LI : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XORCY : component is "BLACK_BOX";
   
   component MUXCY_D
      port ( CI : in    std_logic; 
             DI : in    std_logic; 
             S  : in    std_logic; 
             LO : out   std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of MUXCY_D : component is "BLACK_BOX";
   
   component XOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XOR2 : component is "BLACK_BOX";
   
   attribute RLOC of I_36_16 : label is "X0Y0";
   attribute RLOC of I_36_17 : label is "X0Y0";
   attribute RLOC of I_36_18 : label is "X0Y1";
   attribute RLOC of I_36_19 : label is "X0Y1";
   attribute RLOC of I_36_20 : label is "X0Y2";
   attribute RLOC of I_36_21 : label is "X0Y2";
   attribute RLOC of I_36_22 : label is "X0Y3";
   attribute RLOC of I_36_23 : label is "X0Y3";
   attribute RLOC of I_36_55 : label is "X0Y0";
   attribute RLOC of I_36_58 : label is "X0Y1";
   attribute RLOC of I_36_62 : label is "X0Y1";
   attribute RLOC of I_36_63 : label is "X0Y2";
   attribute RLOC of I_36_64 : label is "X0Y3";
   attribute RLOC of I_36_107 : label is "X0Y3";
   attribute RLOC of I_36_110 : label is "X0Y2";
   attribute RLOC of I_36_111 : label is "X0Y0";
begin
   CO <= CO_DUMMY;
   I_36_16 : FMAP
      port map (I1=>A(0),
                I2=>B(0),
                I3=>dummy,
                I4=>dummy,
                O=>I0);
   
   I_36_17 : FMAP
      port map (I1=>A(1),
                I2=>B(1),
                I3=>dummy,
                I4=>dummy,
                O=>I1);
   
   I_36_18 : FMAP
      port map (I1=>A(2),
                I2=>B(2),
                I3=>dummy,
                I4=>dummy,
                O=>I2);
   
   I_36_19 : FMAP
      port map (I1=>A(3),
                I2=>B(3),
                I3=>dummy,
                I4=>dummy,
                O=>I3);
   
   I_36_20 : FMAP
      port map (I1=>A(4),
                I2=>B(4),
                I3=>dummy,
                I4=>dummy,
                O=>I4);
   
   I_36_21 : FMAP
      port map (I1=>A(5),
                I2=>B(5),
                I3=>dummy,
                I4=>dummy,
                O=>I5);
   
   I_36_22 : FMAP
      port map (I1=>A(6),
                I2=>B(6),
                I3=>dummy,
                I4=>dummy,
                O=>I6);
   
   I_36_23 : FMAP
      port map (I1=>A(7),
                I2=>B(7),
                I3=>dummy,
                I4=>dummy,
                O=>I7);
   
   I_36_55 : MUXCY_L
      port map (CI=>C0,
                DI=>A(1),
                S=>I1,
                LO=>C1);
   
   I_36_58 : MUXCY_L
      port map (CI=>C2,
                DI=>A(3),
                S=>I3,
                LO=>C3);
   
   I_36_62 : MUXCY_L
      port map (CI=>C1,
                DI=>A(2),
                S=>I2,
                LO=>C2);
   
   I_36_63 : MUXCY_L
      port map (CI=>C3,
                DI=>A(4),
                S=>I4,
                LO=>C4);
   
   I_36_64 : MUXCY
      port map (CI=>C6,
                DI=>A(7),
                S=>I7,
                O=>CO_DUMMY);
   
   I_36_73 : XORCY
      port map (CI=>CI,
                LI=>I0,
                O=>S(0));
   
   I_36_74 : XORCY
      port map (CI=>C0,
                LI=>I1,
                O=>S(1));
   
   I_36_75 : XORCY
      port map (CI=>C2,
                LI=>I3,
                O=>S(3));
   
   I_36_76 : XORCY
      port map (CI=>C1,
                LI=>I2,
                O=>S(2));
   
   I_36_77 : XORCY
      port map (CI=>C4,
                LI=>I5,
                O=>S(5));
   
   I_36_78 : XORCY
      port map (CI=>C3,
                LI=>I4,
                O=>S(4));
   
   I_36_80 : XORCY
      port map (CI=>C6,
                LI=>I7,
                O=>S(7));
   
   I_36_81 : XORCY
      port map (CI=>C5,
                LI=>I6,
                O=>S(6));
   
   I_36_107 : MUXCY_D
      port map (CI=>C5,
                DI=>A(6),
                S=>I6,
                LO=>C6,
                O=>C6O);
   
   I_36_110 : MUXCY_L
      port map (CI=>C4,
                DI=>A(5),
                S=>I5,
                LO=>C5);
   
   I_36_111 : MUXCY_L
      port map (CI=>CI,
                DI=>A(0),
                S=>I0,
                LO=>C0);
   
   I_36_221 : XOR2
      port map (I0=>A(7),
                I1=>B(7),
                O=>I7);
   
   I_36_222 : XOR2
      port map (I0=>A(6),
                I1=>B(6),
                O=>I6);
   
   I_36_223 : XOR2
      port map (I0=>A(5),
                I1=>B(5),
                O=>I5);
   
   I_36_224 : XOR2
      port map (I0=>A(4),
                I1=>B(4),
                O=>I4);
   
   I_36_225 : XOR2
      port map (I0=>A(3),
                I1=>B(3),
                O=>I3);
   
   I_36_228 : XOR2
      port map (I0=>A(0),
                I1=>B(0),
                O=>I0);
   
   I_36_229 : XOR2
      port map (I0=>A(1),
                I1=>B(1),
                O=>I1);
   
   I_36_230 : XOR2
      port map (I0=>A(2),
                I1=>B(2),
                O=>I2);
   
   I_36_239 : XOR2
      port map (I0=>C6O,
                I1=>CO_DUMMY,
                O=>OFL);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity M2_1_MXILINX_datapath is
   port ( D0 : in    std_logic; 
          D1 : in    std_logic; 
          S0 : in    std_logic; 
          O  : out   std_logic);
end M2_1_MXILINX_datapath;

architecture BEHAVIORAL of M2_1_MXILINX_datapath is
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

entity BRLSHFT8_MXILINX_datapath is
   port ( I0 : in    std_logic; 
          I1 : in    std_logic; 
          I2 : in    std_logic; 
          I3 : in    std_logic; 
          I4 : in    std_logic; 
          I5 : in    std_logic; 
          I6 : in    std_logic; 
          I7 : in    std_logic; 
          S0 : in    std_logic; 
          S1 : in    std_logic; 
          S2 : in    std_logic; 
          O0 : out   std_logic; 
          O1 : out   std_logic; 
          O2 : out   std_logic; 
          O3 : out   std_logic; 
          O4 : out   std_logic; 
          O5 : out   std_logic; 
          O6 : out   std_logic; 
          O7 : out   std_logic);
end BRLSHFT8_MXILINX_datapath;

architecture BEHAVIORAL of BRLSHFT8_MXILINX_datapath is
   attribute HU_SET     : string ;
   signal MO0 : std_logic;
   signal MO1 : std_logic;
   signal MO2 : std_logic;
   signal MO3 : std_logic;
   signal MO4 : std_logic;
   signal MO5 : std_logic;
   signal MO6 : std_logic;
   signal MO7 : std_logic;
   signal M01 : std_logic;
   signal M12 : std_logic;
   signal M23 : std_logic;
   signal M34 : std_logic;
   signal M45 : std_logic;
   signal M56 : std_logic;
   signal M67 : std_logic;
   signal M70 : std_logic;
   component M2_1_MXILINX_datapath
      port ( D0 : in    std_logic; 
             D1 : in    std_logic; 
             S0 : in    std_logic; 
             O  : out   std_logic);
   end component;
   
   attribute HU_SET of I_MO0 : label is "I_MO0_17";
   attribute HU_SET of I_MO1 : label is "I_MO1_18";
   attribute HU_SET of I_MO2 : label is "I_MO2_19";
   attribute HU_SET of I_MO3 : label is "I_MO3_20";
   attribute HU_SET of I_MO4 : label is "I_MO4_21";
   attribute HU_SET of I_MO5 : label is "I_MO5_22";
   attribute HU_SET of I_MO6 : label is "I_MO6_23";
   attribute HU_SET of I_MO7 : label is "I_MO7_15";
   attribute HU_SET of I_M01 : label is "I_M01_12";
   attribute HU_SET of I_M12 : label is "I_M12_13";
   attribute HU_SET of I_M23 : label is "I_M23_14";
   attribute HU_SET of I_M34 : label is "I_M34_11";
   attribute HU_SET of I_M45 : label is "I_M45_8";
   attribute HU_SET of I_M56 : label is "I_M56_9";
   attribute HU_SET of I_M67 : label is "I_M67_10";
   attribute HU_SET of I_M70 : label is "I_M70_16";
   attribute HU_SET of I_O0 : label is "I_O0_0";
   attribute HU_SET of I_O1 : label is "I_O1_1";
   attribute HU_SET of I_O2 : label is "I_O2_7";
   attribute HU_SET of I_O3 : label is "I_O3_6";
   attribute HU_SET of I_O4 : label is "I_O4_5";
   attribute HU_SET of I_O5 : label is "I_O5_4";
   attribute HU_SET of I_O6 : label is "I_O6_3";
   attribute HU_SET of I_O7 : label is "I_O7_2";
begin
   I_MO0 : M2_1_MXILINX_datapath
      port map (D0=>M01,
                D1=>M23,
                S0=>S1,
                O=>MO0);
   
   I_MO1 : M2_1_MXILINX_datapath
      port map (D0=>M12,
                D1=>M34,
                S0=>S1,
                O=>MO1);
   
   I_MO2 : M2_1_MXILINX_datapath
      port map (D0=>M23,
                D1=>M45,
                S0=>S1,
                O=>MO2);
   
   I_MO3 : M2_1_MXILINX_datapath
      port map (D0=>M34,
                D1=>M56,
                S0=>S1,
                O=>MO3);
   
   I_MO4 : M2_1_MXILINX_datapath
      port map (D0=>M45,
                D1=>M67,
                S0=>S1,
                O=>MO4);
   
   I_MO5 : M2_1_MXILINX_datapath
      port map (D0=>M56,
                D1=>M70,
                S0=>S1,
                O=>MO5);
   
   I_MO6 : M2_1_MXILINX_datapath
      port map (D0=>M67,
                D1=>M01,
                S0=>S1,
                O=>MO6);
   
   I_MO7 : M2_1_MXILINX_datapath
      port map (D0=>M70,
                D1=>M12,
                S0=>S1,
                O=>MO7);
   
   I_M01 : M2_1_MXILINX_datapath
      port map (D0=>I0,
                D1=>I1,
                S0=>S0,
                O=>M01);
   
   I_M12 : M2_1_MXILINX_datapath
      port map (D0=>I1,
                D1=>I2,
                S0=>S0,
                O=>M12);
   
   I_M23 : M2_1_MXILINX_datapath
      port map (D0=>I2,
                D1=>I3,
                S0=>S0,
                O=>M23);
   
   I_M34 : M2_1_MXILINX_datapath
      port map (D0=>I3,
                D1=>I4,
                S0=>S0,
                O=>M34);
   
   I_M45 : M2_1_MXILINX_datapath
      port map (D0=>I4,
                D1=>I5,
                S0=>S0,
                O=>M45);
   
   I_M56 : M2_1_MXILINX_datapath
      port map (D0=>I5,
                D1=>I6,
                S0=>S0,
                O=>M56);
   
   I_M67 : M2_1_MXILINX_datapath
      port map (D0=>I6,
                D1=>I7,
                S0=>S0,
                O=>M67);
   
   I_M70 : M2_1_MXILINX_datapath
      port map (D0=>I7,
                D1=>I0,
                S0=>S0,
                O=>M70);
   
   I_O0 : M2_1_MXILINX_datapath
      port map (D0=>MO0,
                D1=>MO4,
                S0=>S2,
                O=>O0);
   
   I_O1 : M2_1_MXILINX_datapath
      port map (D0=>MO1,
                D1=>MO5,
                S0=>S2,
                O=>O1);
   
   I_O2 : M2_1_MXILINX_datapath
      port map (D0=>MO2,
                D1=>MO6,
                S0=>S2,
                O=>O2);
   
   I_O3 : M2_1_MXILINX_datapath
      port map (D0=>MO3,
                D1=>MO7,
                S0=>S2,
                O=>O3);
   
   I_O4 : M2_1_MXILINX_datapath
      port map (D0=>MO4,
                D1=>MO0,
                S0=>S2,
                O=>O4);
   
   I_O5 : M2_1_MXILINX_datapath
      port map (D0=>MO5,
                D1=>MO1,
                S0=>S2,
                O=>O5);
   
   I_O6 : M2_1_MXILINX_datapath
      port map (D0=>MO6,
                D1=>MO2,
                S0=>S2,
                O=>O6);
   
   I_O7 : M2_1_MXILINX_datapath
      port map (D0=>MO7,
                D1=>MO3,
                S0=>S2,
                O=>O7);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity COMPM8_MXILINX_datapath is
   port ( A  : in    std_logic_vector (7 downto 0); 
          B  : in    std_logic_vector (7 downto 0); 
          GT : out   std_logic; 
          LT : out   std_logic);
end COMPM8_MXILINX_datapath;

architecture BEHAVIORAL of COMPM8_MXILINX_datapath is
   attribute BOX_TYPE   : string ;
   signal EQ_1  : std_logic;
   signal EQ_3  : std_logic;
   signal EQ_5  : std_logic;
   signal EQ_7  : std_logic;
   signal EQ2_3 : std_logic;
   signal EQ4_5 : std_logic;
   signal EQ6_7 : std_logic;
   signal GE0_1 : std_logic;
   signal GE2_3 : std_logic;
   signal GE4_5 : std_logic;
   signal GE6_7 : std_logic;
   signal GTA   : std_logic;
   signal GTB   : std_logic;
   signal GTC   : std_logic;
   signal GTD   : std_logic;
   signal GT_1  : std_logic;
   signal GT_3  : std_logic;
   signal GT_5  : std_logic;
   signal GT_7  : std_logic;
   signal GT0_1 : std_logic;
   signal GT2_3 : std_logic;
   signal GT4_5 : std_logic;
   signal LE0_1 : std_logic;
   signal LE2_3 : std_logic;
   signal LE4_5 : std_logic;
   signal LE6_7 : std_logic;
   signal LTA   : std_logic;
   signal LTB   : std_logic;
   signal LTC   : std_logic;
   signal LTD   : std_logic;
   signal LT_1  : std_logic;
   signal LT_3  : std_logic;
   signal LT_5  : std_logic;
   signal LT_7  : std_logic;
   signal LT0_1 : std_logic;
   signal LT2_3 : std_logic;
   signal LT4_5 : std_logic;
   component AND2B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2B1 : component is "BLACK_BOX";
   
   component OR4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR4 : component is "BLACK_BOX";
   
   component NOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of NOR2 : component is "BLACK_BOX";
   
   component AND3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3 : component is "BLACK_BOX";
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
   component AND3B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B1 : component is "BLACK_BOX";
   
   component XNOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XNOR2 : component is "BLACK_BOX";
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
   component AND4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4 : component is "BLACK_BOX";
   
begin
   I_36_1 : AND2B1
      port map (I0=>A(7),
                I1=>B(7),
                O=>LT_7);
   
   I_36_3 : OR4
      port map (I0=>LTD,
                I1=>LTC,
                I2=>LTB,
                I3=>LTA,
                O=>LT);
   
   I_36_4 : NOR2
      port map (I0=>GT4_5,
                I1=>LT4_5,
                O=>EQ4_5);
   
   I_36_5 : AND3
      port map (I0=>EQ6_7,
                I1=>EQ4_5,
                I2=>LT2_3,
                O=>LTB);
   
   I_36_6 : OR2
      port map (I0=>LT_3,
                I1=>LE2_3,
                O=>LT2_3);
   
   I_36_7 : OR2
      port map (I0=>GT_3,
                I1=>GE2_3,
                O=>GT2_3);
   
   I_36_8 : OR2
      port map (I0=>GT_1,
                I1=>GE0_1,
                O=>GT0_1);
   
   I_36_9 : AND3B1
      port map (I0=>A(0),
                I1=>EQ_1,
                I2=>B(0),
                O=>LE0_1);
   
   I_36_10 : AND3B1
      port map (I0=>B(0),
                I1=>EQ_1,
                I2=>A(0),
                O=>GE0_1);
   
   I_36_11 : AND3B1
      port map (I0=>A(2),
                I1=>EQ_3,
                I2=>B(2),
                O=>LE2_3);
   
   I_36_12 : AND3B1
      port map (I0=>B(2),
                I1=>EQ_3,
                I2=>A(2),
                O=>GE2_3);
   
   I_36_13 : XNOR2
      port map (I0=>A(1),
                I1=>B(1),
                O=>EQ_1);
   
   I_36_14 : AND2B1
      port map (I0=>B(1),
                I1=>A(1),
                O=>GT_1);
   
   I_36_15 : XNOR2
      port map (I0=>A(3),
                I1=>B(3),
                O=>EQ_3);
   
   I_36_16 : AND2B1
      port map (I0=>A(1),
                I1=>B(1),
                O=>LT_1);
   
   I_36_17 : AND2B1
      port map (I0=>B(3),
                I1=>A(3),
                O=>GT_3);
   
   I_36_18 : OR2
      port map (I0=>LT_1,
                I1=>LE0_1,
                O=>LT0_1);
   
   I_36_19 : AND2B1
      port map (I0=>A(3),
                I1=>B(3),
                O=>LT_3);
   
   I_36_20 : AND2
      port map (I0=>GT4_5,
                I1=>EQ6_7,
                O=>GTC);
   
   I_36_21 : AND2
      port map (I0=>EQ6_7,
                I1=>LT4_5,
                O=>LTC);
   
   I_36_22 : NOR2
      port map (I0=>GTD,
                I1=>LTD,
                O=>EQ6_7);
   
   I_36_23 : OR2
      port map (I0=>LT_5,
                I1=>LE4_5,
                O=>LT4_5);
   
   I_36_24 : AND2B1
      port map (I0=>B(7),
                I1=>A(7),
                O=>GT_7);
   
   I_36_25 : AND2B1
      port map (I0=>A(5),
                I1=>B(5),
                O=>LT_5);
   
   I_36_26 : XNOR2
      port map (I0=>A(7),
                I1=>B(7),
                O=>EQ_7);
   
   I_36_27 : AND2B1
      port map (I0=>B(5),
                I1=>A(5),
                O=>GT_5);
   
   I_36_28 : XNOR2
      port map (I0=>A(5),
                I1=>B(5),
                O=>EQ_5);
   
   I_36_29 : AND3B1
      port map (I0=>B(6),
                I1=>EQ_7,
                I2=>A(6),
                O=>GE6_7);
   
   I_36_30 : AND3B1
      port map (I0=>A(6),
                I1=>EQ_7,
                I2=>B(6),
                O=>LE6_7);
   
   I_36_31 : AND3B1
      port map (I0=>B(4),
                I1=>EQ_5,
                I2=>A(4),
                O=>GE4_5);
   
   I_36_32 : AND3B1
      port map (I0=>A(4),
                I1=>EQ_5,
                I2=>B(4),
                O=>LE4_5);
   
   I_36_33 : OR2
      port map (I0=>GT_5,
                I1=>GE4_5,
                O=>GT4_5);
   
   I_36_34 : OR2
      port map (I0=>GT_7,
                I1=>GE6_7,
                O=>GTD);
   
   I_36_35 : OR2
      port map (I0=>LT_7,
                I1=>LE6_7,
                O=>LTD);
   
   I_36_36 : AND4
      port map (I0=>EQ6_7,
                I1=>EQ4_5,
                I2=>EQ2_3,
                I3=>LT0_1,
                O=>LTA);
   
   I_36_37 : AND4
      port map (I0=>GT0_1,
                I1=>EQ2_3,
                I2=>EQ4_5,
                I3=>EQ6_7,
                O=>GTA);
   
   I_36_38 : AND3
      port map (I0=>GT2_3,
                I1=>EQ4_5,
                I2=>EQ6_7,
                O=>GTB);
   
   I_36_39 : NOR2
      port map (I0=>GT2_3,
                I1=>LT2_3,
                O=>EQ2_3);
   
   I_36_40 : OR4
      port map (I0=>GTD,
                I1=>GTC,
                I2=>GTB,
                I3=>GTA,
                O=>GT);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity datapath is
   port ( R1      : in    std_logic_vector (4 downto 0); 
          R2      : in    std_logic_vector (4 downto 0); 
          sel     : in    std_logic_vector (1 downto 0); 
          shf_of  : in    std_logic_vector (1 downto 0); 
          dpath_D : out   std_logic_vector (4 downto 0); 
          GE_FLG  : out   std_logic; 
          OV_FLG  : out   std_logic);
end datapath;

architecture BEHAVIORAL of datapath is
   attribute BOX_TYPE   : string ;
   attribute HU_SET     : string ;
   signal A        : std_logic_vector (7 downto 0);
   signal B        : std_logic_vector (7 downto 0);
   signal C        : std_logic_vector (4 downto 0);
   signal O        : std_logic_vector (4 downto 0);
   signal S        : std_logic_vector (7 downto 0);
   signal XLXN_5   : std_logic;
   signal XLXN_33  : std_logic;
   signal XLXN_95  : std_logic;
   signal XLXN_249 : std_logic;
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component ADD8_MXILINX_datapath
      port ( A   : in    std_logic_vector (7 downto 0); 
             B   : in    std_logic_vector (7 downto 0); 
             CI  : in    std_logic; 
             CO  : out   std_logic; 
             OFL : out   std_logic; 
             S   : out   std_logic_vector (7 downto 0));
   end component;
   
   component BRLSHFT8_MXILINX_datapath
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             I4 : in    std_logic; 
             I5 : in    std_logic; 
             I6 : in    std_logic; 
             I7 : in    std_logic; 
             S0 : in    std_logic; 
             S1 : in    std_logic; 
             S2 : in    std_logic; 
             O0 : out   std_logic; 
             O1 : out   std_logic; 
             O2 : out   std_logic; 
             O3 : out   std_logic; 
             O4 : out   std_logic; 
             O5 : out   std_logic; 
             O6 : out   std_logic; 
             O7 : out   std_logic);
   end component;
   
   component COMPM8_MXILINX_datapath
      port ( A  : in    std_logic_vector (7 downto 0); 
             B  : in    std_logic_vector (7 downto 0); 
             GT : out   std_logic; 
             LT : out   std_logic);
   end component;
   
   component Mask5
      port ( A : in    std_logic_vector (4 downto 0); 
             B : in    std_logic_vector (4 downto 0); 
             C : out   std_logic_vector (4 downto 0));
   end component;
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component BUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of BUF : component is "BLACK_BOX";
   
   component Mux3_5bus_v2
      port ( C   : in    std_logic_vector (4 downto 0); 
             A   : in    std_logic_vector (4 downto 0); 
             B   : in    std_logic_vector (4 downto 0); 
             O   : out   std_logic_vector (4 downto 0); 
             sel : in    std_logic_vector (1 downto 0));
   end component;
   
   attribute HU_SET of XLXI_2 : label is "XLXI_2_26";
   attribute HU_SET of XLXI_3 : label is "XLXI_3_25";
   attribute HU_SET of XLXI_4 : label is "XLXI_4_24";
begin
   gndA_0 : GND
      port map (G=>A(5));
   
   gndA_1 : GND
      port map (G=>A(6));
   
   gndA_2 : GND
      port map (G=>A(7));
   
   gndB_0 : GND
      port map (G=>B(5));
   
   gndB_1 : GND
      port map (G=>B(6));
   
   gndB_2 : GND
      port map (G=>B(7));
   
   XLXI_2 : ADD8_MXILINX_datapath
      port map (A(7 downto 0)=>A(7 downto 0),
                B(7 downto 0)=>B(7 downto 0),
                CI=>XLXN_95,
                CO=>open,
                OFL=>OV_FLG,
                S(7 downto 0)=>S(7 downto 0));
   
   XLXI_3 : BRLSHFT8_MXILINX_datapath
      port map (I0=>R2(4),
                I1=>R2(3),
                I2=>R2(2),
                I3=>R2(1),
                I4=>R2(0),
                I5=>XLXN_249,
                I6=>XLXN_249,
                I7=>XLXN_249,
                S0=>shf_of(0),
                S1=>shf_of(1),
                S2=>XLXN_5,
                O0=>O(4),
                O1=>O(3),
                O2=>O(2),
                O3=>O(1),
                O4=>O(0),
                O5=>open,
                O6=>open,
                O7=>open);
   
   XLXI_4 : COMPM8_MXILINX_datapath
      port map (A(7 downto 0)=>A(7 downto 0),
                B(7 downto 0)=>B(7 downto 0),
                GT=>open,
                LT=>XLXN_33);
   
   XLXI_5 : GND
      port map (G=>XLXN_249);
   
   XLXI_6 : Mask5
      port map (A(4 downto 0)=>R1(4 downto 0),
                B(4 downto 0)=>R2(4 downto 0),
                C(4 downto 0)=>C(4 downto 0));
   
   XLXI_7 : INV
      port map (I=>XLXN_33,
                O=>GE_FLG);
   
   XLXI_9_0 : BUF
      port map (I=>R1(0),
                O=>A(0));
   
   XLXI_9_1 : BUF
      port map (I=>R1(1),
                O=>A(1));
   
   XLXI_9_2 : BUF
      port map (I=>R1(2),
                O=>A(2));
   
   XLXI_9_3 : BUF
      port map (I=>R1(3),
                O=>A(3));
   
   XLXI_9_4 : BUF
      port map (I=>R1(4),
                O=>A(4));
   
   XLXI_16_0 : BUF
      port map (I=>R2(0),
                O=>B(0));
   
   XLXI_16_1 : BUF
      port map (I=>R2(1),
                O=>B(1));
   
   XLXI_16_2 : BUF
      port map (I=>R2(2),
                O=>B(2));
   
   XLXI_16_3 : BUF
      port map (I=>R2(3),
                O=>B(3));
   
   XLXI_16_4 : BUF
      port map (I=>R2(4),
                O=>B(4));
   
   XLXI_33 : GND
      port map (G=>XLXN_95);
   
   XLXI_54 : GND
      port map (G=>XLXN_5);
   
   XLXI_55 : Mux3_5bus_v2
      port map (A(4 downto 0)=>O(4 downto 0),
                B(4 downto 0)=>S(4 downto 0),
                C(4 downto 0)=>C(4 downto 0),
                sel(1 downto 0)=>sel(1 downto 0),
                O(4 downto 0)=>dpath_D(4 downto 0));
   
end BEHAVIORAL;


