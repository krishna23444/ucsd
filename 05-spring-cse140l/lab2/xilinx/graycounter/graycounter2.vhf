--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : graycounter2.vhf
-- /___/   /\     Timestamp : 05/04/2005 00:58:44
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w graycounter2.sch graycounter2.vhf
--Design Name: graycounter2
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

entity FTCE_MXILINX_graycounter2 is
   port ( C   : in    std_logic; 
          CE  : in    std_logic; 
          CLR : in    std_logic; 
          T   : in    std_logic; 
          Q   : out   std_logic);
end FTCE_MXILINX_graycounter2;

architecture BEHAVIORAL of FTCE_MXILINX_graycounter2 is
   attribute BOX_TYPE   : string ;
   attribute INIT       : string ;
   attribute RLOC       : string ;
   signal TQ      : std_logic;
   signal Q_DUMMY : std_logic;
   component XOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XOR2 : component is "BLACK_BOX";
   
   component FDCE
      -- synopsys translate_off
      generic( INIT : bit :=  '0');
      -- synopsys translate_on
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute INIT of FDCE : component is "0";
   attribute BOX_TYPE of FDCE : component is "BLACK_BOX";
   
   attribute RLOC of I_36_35 : label is "X0Y0";
begin
   Q <= Q_DUMMY;
   I_36_32 : XOR2
      port map (I0=>T,
                I1=>Q_DUMMY,
                O=>TQ);
   
   I_36_35 : FDCE
      port map (C=>C,
                CE=>CE,
                CLR=>CLR,
                D=>TQ,
                Q=>Q_DUMMY);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity graycounter2 is
   port ( CLK    : in    std_logic; 
          CLK_CE : in    std_logic; 
          CLR    : in    std_logic; 
          D      : out   std_logic_vector (3 downto 0));
end graycounter2;

architecture BEHAVIORAL of graycounter2 is
   attribute HU_SET     : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_32 : std_logic;
   signal XLXN_33 : std_logic;
   signal XLXN_37 : std_logic;
   signal XLXN_38 : std_logic;
   signal XLXN_39 : std_logic;
   signal XLXN_41 : std_logic;
   signal XLXN_43 : std_logic;
   signal XLXN_46 : std_logic;
   signal XLXN_51 : std_logic;
   signal D_DUMMY : std_logic_vector (3 downto 0);
   component FTCE_MXILINX_graycounter2
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             T   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   
   component XNOR4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XNOR4 : component is "BLACK_BOX";
   
   component XOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XOR2 : component is "BLACK_BOX";
   
   component AND3B2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B2 : component is "BLACK_BOX";
   
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
   
   attribute HU_SET of XLXI_1 : label is "XLXI_1_2";
   attribute HU_SET of XLXI_2 : label is "XLXI_2_1";
   attribute HU_SET of XLXI_3 : label is "XLXI_3_3";
   attribute HU_SET of XLXI_4 : label is "XLXI_4_0";
begin
   D(3 downto 0) <= D_DUMMY(3 downto 0);
   XLXI_1 : FTCE_MXILINX_graycounter2
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_33,
                Q=>D_DUMMY(0));
   
   XLXI_2 : FTCE_MXILINX_graycounter2
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_32,
                Q=>D_DUMMY(1));
   
   XLXI_3 : FTCE_MXILINX_graycounter2
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_38,
                Q=>D_DUMMY(2));
   
   XLXI_4 : FTCE_MXILINX_graycounter2
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_41,
                Q=>D_DUMMY(3));
   
   XLXI_27 : XNOR4
      port map (I0=>D_DUMMY(0),
                I1=>D_DUMMY(1),
                I2=>D_DUMMY(2),
                I3=>D_DUMMY(3),
                O=>XLXN_33);
   
   XLXI_30 : XOR2
      port map (I0=>D_DUMMY(3),
                I1=>D_DUMMY(2),
                O=>XLXN_37);
   
   XLXI_31 : AND3B2
      port map (I0=>XLXN_37,
                I1=>D_DUMMY(0),
                I2=>D_DUMMY(1),
                O=>XLXN_38);
   
   XLXI_32 : XOR2
      port map (I0=>D_DUMMY(3),
                I1=>D_DUMMY(2),
                O=>XLXN_39);
   
   XLXI_33 : AND3B2
      port map (I0=>D_DUMMY(1),
                I1=>D_DUMMY(0),
                I2=>XLXN_39,
                O=>XLXN_41);
   
   XLXI_41 : XOR2
      port map (I0=>D_DUMMY(1),
                I1=>D_DUMMY(2),
                O=>XLXN_51);
   
   XLXI_43 : AND3B2
      port map (I0=>XLXN_51,
                I1=>D_DUMMY(3),
                I2=>D_DUMMY(0),
                O=>XLXN_46);
   
   XLXI_44 : AND3
      port map (I0=>XLXN_51,
                I1=>D_DUMMY(0),
                I2=>D_DUMMY(3),
                O=>XLXN_43);
   
   XLXI_45 : OR2
      port map (I0=>XLXN_43,
                I1=>XLXN_46,
                O=>XLXN_32);
   
end BEHAVIORAL;


