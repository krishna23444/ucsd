--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : regs.vhf
-- /___/   /\     Timestamp : 05/31/2005 23:48:59
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w regs.sch regs.vhf
--Design Name: regs
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

entity regs is
   port ( clk     : in    std_logic; 
          clr     : in    std_logic; 
          dpath_D : in    std_logic_vector (4 downto 0); 
          GE      : in    std_logic; 
          ge_en   : in    std_logic; 
          mem_D   : in    std_logic_vector (4 downto 0); 
          OV      : in    std_logic; 
          ov_en   : in    std_logic; 
          R1_en   : in    std_logic; 
          R2_en   : in    std_logic; 
          R2_sel  : in    std_logic; 
          R3_en   : in    std_logic; 
          GEF     : out   std_logic; 
          OVF     : out   std_logic; 
          R1      : out   std_logic_vector (4 downto 0); 
          R2      : out   std_logic_vector (4 downto 0); 
          R3      : out   std_logic_vector (4 downto 0));
end regs;

architecture BEHAVIORAL of regs is
   attribute INIT       : string ;
   attribute BOX_TYPE   : string ;
   signal XLXN_1   : std_logic_vector (4 downto 0);
   signal R3_DUMMY : std_logic_vector (4 downto 0);
   component FDRE
      -- synopsys translate_off
      generic( INIT : bit :=  '0');
      -- synopsys translate_on
      port ( C  : in    std_logic; 
             CE : in    std_logic; 
             D  : in    std_logic; 
             R  : in    std_logic; 
             Q  : out   std_logic);
   end component;
   attribute INIT of FDRE : component is "0";
   attribute BOX_TYPE of FDRE : component is "BLACK_BOX";
   
   component Register5
      port ( D      : in    std_logic_vector (4 downto 0); 
             clk_en : in    std_logic; 
             clk    : in    std_logic; 
             clr    : in    std_logic; 
             Q      : out   std_logic_vector (4 downto 0));
   end component;
   
   component Mux2_5bus
      port ( A : in    std_logic_vector (4 downto 0); 
             B : in    std_logic_vector (4 downto 0); 
             C : out   std_logic_vector (4 downto 0); 
             S : in    std_logic);
   end component;
   
begin
   R3(4 downto 0) <= R3_DUMMY(4 downto 0);
   GE_FLG : FDRE
      port map (C=>clk,
                CE=>ge_en,
                D=>GE,
                R=>clr,
                Q=>GEF);
   
   OV_FLG : FDRE
      port map (C=>clk,
                CE=>ov_en,
                D=>OV,
                R=>clr,
                Q=>OVF);
   
   Reg1 : Register5
      port map (clk=>clk,
                clk_en=>R1_en,
                clr=>clr,
                D(4 downto 0)=>mem_D(4 downto 0),
                Q(4 downto 0)=>R1(4 downto 0));
   
   Reg2 : Register5
      port map (clk=>clk,
                clk_en=>R2_en,
                clr=>clr,
                D(4 downto 0)=>XLXN_1(4 downto 0),
                Q(4 downto 0)=>R2(4 downto 0));
   
   Reg3 : Register5
      port map (clk=>clk,
                clk_en=>R3_en,
                clr=>clr,
                D(4 downto 0)=>dpath_D(4 downto 0),
                Q(4 downto 0)=>R3_DUMMY(4 downto 0));
   
   XLXI_4 : Mux2_5bus
      port map (A(4 downto 0)=>mem_D(4 downto 0),
                B(4 downto 0)=>R3_DUMMY(4 downto 0),
                S=>R2_sel,
                C(4 downto 0)=>XLXN_1(4 downto 0));
   
end BEHAVIORAL;


