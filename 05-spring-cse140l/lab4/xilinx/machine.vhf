--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : sch2vhdl
--  /   /         Filename : machine.vhf
-- /___/   /\     Timestamp : 06/01/2005 15:11:01
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: G:/xilinx/ise71/bin/nt/sch2vhdl.exe -intstyle ise -family virtex2 -flat -suppress -w machine.sch machine.vhf
--Design Name: machine
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

entity machine is
   port ( clk    : in    std_logic; 
          clk_en : in    std_logic; 
          rst    : in    std_logic; 
          GE_FLG : out   std_logic; 
          inst   : out   std_logic_vector (7 downto 0); 
          OV_FLG : out   std_logic; 
          pc     : out   std_logic_vector (3 downto 0); 
          R1     : out   std_logic_vector (4 downto 0); 
          R2     : out   std_logic_vector (4 downto 0); 
          R3     : out   std_logic_vector (4 downto 0));
end machine;

architecture BEHAVIORAL of machine is
   signal sel        : std_logic_vector (1 downto 0);
   signal shf_of     : std_logic_vector (1 downto 0);
   signal XLXN_23    : std_logic;
   signal XLXN_24    : std_logic;
   signal XLXN_25    : std_logic;
   signal XLXN_26    : std_logic;
   signal XLXN_27    : std_logic_vector (4 downto 0);
   signal XLXN_28    : std_logic;
   signal XLXN_29    : std_logic;
   signal XLXN_30    : std_logic;
   signal XLXN_31    : std_logic;
   signal XLXN_32    : std_logic;
   signal inst_DUMMY : std_logic_vector (7 downto 0);
   signal R1_DUMMY   : std_logic_vector (4 downto 0);
   signal R2_DUMMY   : std_logic_vector (4 downto 0);
   signal pc_DUMMY   : std_logic_vector (3 downto 0);
   component datapath
      port ( R1      : in    std_logic_vector (4 downto 0); 
             R2      : in    std_logic_vector (4 downto 0); 
             sel     : in    std_logic_vector (1 downto 0); 
             shf_of  : in    std_logic_vector (1 downto 0); 
             GE_FLG  : out   std_logic; 
             OV_FLG  : out   std_logic; 
             dpath_D : out   std_logic_vector (4 downto 0));
   end component;
   
   component regs
      port ( mem_D   : in    std_logic_vector (4 downto 0); 
             R1_en   : in    std_logic; 
             clk     : in    std_logic; 
             clr     : in    std_logic; 
             R2_sel  : in    std_logic; 
             R2_en   : in    std_logic; 
             R3_en   : in    std_logic; 
             OV      : in    std_logic; 
             ov_en   : in    std_logic; 
             GE      : in    std_logic; 
             ge_en   : in    std_logic; 
             R1      : out   std_logic_vector (4 downto 0); 
             R2      : out   std_logic_vector (4 downto 0); 
             R3      : out   std_logic_vector (4 downto 0); 
             OVF     : out   std_logic; 
             GEF     : out   std_logic; 
             dpath_D : in    std_logic_vector (4 downto 0));
   end component;
   
   component pcounter
      port ( clk_en : in    std_logic; 
             clk    : in    std_logic; 
             clr    : in    std_logic; 
             addr   : out   std_logic_vector (3 downto 0));
   end component;
   
   component memory16x8
      port ( addr : in    std_logic_vector (3 downto 0); 
             data : out   std_logic_vector (7 downto 0));
   end component;
   
   component truthtbl
      port ( D      : in    std_logic_vector (7 downto 3); 
             clr    : out   std_logic; 
             R1_en  : out   std_logic; 
             R2_en  : out   std_logic; 
             R2_sel : out   std_logic; 
             R3_en  : out   std_logic; 
             ov_en  : out   std_logic; 
             ge_en  : out   std_logic; 
             sel    : out   std_logic_vector (1 downto 0); 
             shf_of : out   std_logic_vector (1 downto 0));
   end component;
   
begin
   inst(7 downto 0) <= inst_DUMMY(7 downto 0);
   pc(3 downto 0) <= pc_DUMMY(3 downto 0);
   R1(4 downto 0) <= R1_DUMMY(4 downto 0);
   R2(4 downto 0) <= R2_DUMMY(4 downto 0);
   XLXI_1 : datapath
      port map (R1(4 downto 0)=>R1_DUMMY(4 downto 0),
                R2(4 downto 0)=>R2_DUMMY(4 downto 0),
                sel(1 downto 0)=>sel(1 downto 0),
                shf_of(1 downto 0)=>shf_of(1 downto 0),
                dpath_D(4 downto 0)=>XLXN_27(4 downto 0),
                GE_FLG=>XLXN_25,
                OV_FLG=>XLXN_26);
   
   XLXI_2 : regs
      port map (clk=>clk,
                clr=>XLXN_28,
                dpath_D(4 downto 0)=>XLXN_27(4 downto 0),
                GE=>XLXN_25,
                ge_en=>XLXN_23,
                mem_D(4 downto 0)=>inst_Dummy(4 downto 0),
                OV=>XLXN_26,
                ov_en=>XLXN_24,
                R1_en=>XLXN_29,
                R2_en=>XLXN_30,
                R2_sel=>XLXN_31,
                R3_en=>XLXN_32,
                GEF=>GE_FLG,
                OVF=>OV_FLG,
                R1(4 downto 0)=>R1_DUMMY(4 downto 0),
                R2(4 downto 0)=>R2_DUMMY(4 downto 0),
                R3(4 downto 0)=>R3(4 downto 0));
   
   XLXI_3 : pcounter
      port map (clk=>clk,
                clk_en=>clk_en,
                clr=>rst,
                addr(3 downto 0)=>pc_DUMMY(3 downto 0));
   
   XLXI_4 : memory16x8
      port map (addr(3 downto 0)=>pc_DUMMY(3 downto 0),
                data(7 downto 0)=>inst_DUMMY(7 downto 0));
   
   XLXI_5 : truthtbl
      port map (D(7 downto 3)=>inst_Dummy(7 downto 3),
                clr=>XLXN_28,
                ge_en=>XLXN_23,
                ov_en=>XLXN_24,
                R1_en=>XLXN_29,
                R2_en=>XLXN_30,
                R2_sel=>XLXN_31,
                R3_en=>XLXN_32,
                sel(1 downto 0)=>sel(1 downto 0),
                shf_of(1 downto 0)=>shf_of(1 downto 0));
   
end BEHAVIORAL;


