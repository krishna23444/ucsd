--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : graycounter.vhf
-- /___/   /\     Timestamp : 05/03/2005 22:42:35
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w graycounter.sch graycounter.vhf
--Design Name: graycounter
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

entity FTCE_MXILINX_graycounter is
   port ( C   : in    std_logic; 
          CE  : in    std_logic; 
          CLR : in    std_logic; 
          T   : in    std_logic; 
          Q   : out   std_logic);
end FTCE_MXILINX_graycounter;

architecture BEHAVIORAL of FTCE_MXILINX_graycounter is
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

entity graycounter is
   port ( CLK    : in    std_logic; 
          CLK_CE : in    std_logic; 
          CLR    : in    std_logic; 
          D      : out   std_logic_vector (3 downto 0));
end graycounter;

architecture BEHAVIORAL of graycounter is
   attribute HU_SET     : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_28 : std_logic;
   signal XLXN_29 : std_logic;
   signal XLXN_30 : std_logic;
   signal XLXN_31 : std_logic;
   signal XLXN_32 : std_logic;
   signal XLXN_33 : std_logic;
   signal XLXN_37 : std_logic;
   signal XLXN_38 : std_logic;
   signal XLXN_39 : std_logic;
   signal XLXN_41 : std_logic;
   signal D_DUMMY : std_logic_vector (3 downto 0);
   component FTCE_MXILINX_graycounter
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             T   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   
   component AND4B3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B3 : component is "BLACK_BOX";
   
   component AND4B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B1 : component is "BLACK_BOX";
   
   component OR4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR4 : component is "BLACK_BOX";
   
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
   
   attribute HU_SET of XLXI_1 : label is "XLXI_1_2";
   attribute HU_SET of XLXI_2 : label is "XLXI_2_1";
   attribute HU_SET of XLXI_3 : label is "XLXI_3_3";
   attribute HU_SET of XLXI_4 : label is "XLXI_4_0";
begin
   D(3 downto 0) <= D_DUMMY(3 downto 0);
   XLXI_1 : FTCE_MXILINX_graycounter
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_33,
                Q=>D_DUMMY(0));
   
   XLXI_2 : FTCE_MXILINX_graycounter
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_32,
                Q=>D_DUMMY(1));
   
   XLXI_3 : FTCE_MXILINX_graycounter
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_38,
                Q=>D_DUMMY(2));
   
   XLXI_4 : FTCE_MXILINX_graycounter
      port map (C=>CLK,
                CE=>CLK_CE,
                CLR=>CLR,
                T=>XLXN_41,
                Q=>D_DUMMY(3));
   
   XLXI_15 : AND4B3
      port map (I0=>D_DUMMY(3),
                I1=>D_DUMMY(2),
                I2=>D_DUMMY(1),
                I3=>D_DUMMY(0),
                O=>XLXN_28);
   
   XLXI_23 : AND4B1
      port map (I0=>D_DUMMY(2),
                I1=>D_DUMMY(1),
                I2=>D_DUMMY(0),
                I3=>D_DUMMY(3),
                O=>XLXN_29);
   
   XLXI_24 : AND4B1
      port map (I0=>D_DUMMY(1),
                I1=>D_DUMMY(3),
                I2=>D_DUMMY(2),
                I3=>D_DUMMY(0),
                O=>XLXN_30);
   
   XLXI_25 : AND4B1
      port map (I0=>D_DUMMY(3),
                I1=>D_DUMMY(2),
                I2=>D_DUMMY(1),
                I3=>D_DUMMY(0),
                O=>XLXN_31);
   
   XLXI_26 : OR4
      port map (I0=>XLXN_28,
                I1=>XLXN_29,
                I2=>XLXN_30,
                I3=>XLXN_31,
                O=>XLXN_32);
   
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
   
end BEHAVIORAL;


