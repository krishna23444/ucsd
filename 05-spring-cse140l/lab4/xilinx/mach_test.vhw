--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : ISE WebPACK
--  /   /         Filename : mach_test.vhw
-- /___/   /\     Timestamp : Wed Jun 01 14:06:48 2005
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: mach_test
--Device: Xilinx
--

library UNISIM;
use UNISIM.Vcomponents.ALL;
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY mach_test IS
END mach_test;

ARCHITECTURE testbench_arch OF mach_test IS
    COMPONENT machine
        PORT (
            clk : In std_logic;
            clk_en : In std_logic;
            rst : In std_logic;
            GE_FLG : Out std_logic;
            inst : Out std_logic_vector (7 DownTo 0);
            OV_FLG : Out std_logic;
            pc : Out std_logic_vector (3 DownTo 0);
            R1 : Out std_logic_vector (4 DownTo 0);
            R2 : Out std_logic_vector (4 DownTo 0);
            R3 : Out std_logic_vector (4 DownTo 0)
        );
    END COMPONENT;

    SIGNAL clk : std_logic := '0';
    SIGNAL clk_en : std_logic := '0';
    SIGNAL rst : std_logic := '0';
    SIGNAL GE_FLG : std_logic := '0';
    SIGNAL inst : std_logic_vector (7 DownTo 0) := "UUUUUUUU";
    SIGNAL OV_FLG : std_logic := '0';
    SIGNAL pc : std_logic_vector (3 DownTo 0) := "UUUU";
    SIGNAL R1 : std_logic_vector (4 DownTo 0) := "UUUUU";
    SIGNAL R2 : std_logic_vector (4 DownTo 0) := "UUUUU";
    SIGNAL R3 : std_logic_vector (4 DownTo 0) := "UUUUU";

    SHARED VARIABLE TX_ERROR : INTEGER := 0;
    SHARED VARIABLE TX_OUT : LINE;

    constant PERIOD : time := 20 ns;
    constant DUTY_CYCLE : real := 0.5;
    constant OFFSET : time := 0 ns;

    BEGIN
        UUT : machine
        PORT MAP (
            clk => clk,
            clk_en => clk_en,
            rst => rst,
            GE_FLG => GE_FLG,
            inst => inst,
            OV_FLG => OV_FLG,
            pc => pc,
            R1 => R1,
            R2 => R2,
            R3 => R3
        );

        PROCESS    -- clock process for clk
        BEGIN
            WAIT for OFFSET;
            CLOCK_LOOP : LOOP
                clk <= '0';
                WAIT FOR (PERIOD - (PERIOD * DUTY_CYCLE));
                clk <= '1';
                WAIT FOR (PERIOD * DUTY_CYCLE);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS
            PROCEDURE CHECK_GE_FLG(
                next_GE_FLG : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (GE_FLG /= next_GE_FLG) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns GE_FLG="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, GE_FLG);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_GE_FLG);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_OV_FLG(
                next_OV_FLG : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (OV_FLG /= next_OV_FLG) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns OV_FLG="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, OV_FLG);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_OV_FLG);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_R1(
                next_R1 : std_logic_vector (4 DownTo 0);
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (R1 /= next_R1) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns R1="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, R1);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_R1);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_R2(
                next_R2 : std_logic_vector (4 DownTo 0);
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (R2 /= next_R2) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns R2="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, R2);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_R2);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_R3(
                next_R3 : std_logic_vector (4 DownTo 0);
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (R3 /= next_R3) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns R3="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, R3);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_R3);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_inst(
                next_inst : std_logic_vector (7 DownTo 0);
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (inst /= next_inst) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns inst="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, inst);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_inst);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_pc(
                next_pc : std_logic_vector (3 DownTo 0);
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (pc /= next_pc) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns pc="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, pc);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_pc);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            BEGIN
                -- -------------  Current Time:  7ns
                WAIT FOR 7 ns;
                rst <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  13ns
                WAIT FOR 6 ns;
                CHECK_inst("00000000", 13);
                CHECK_pc("0000", 13);
                CHECK_R1("00000", 13);
                CHECK_R2("00000", 13);
                CHECK_R3("00000", 13);
                -- -------------------------------------
                -- -------------  Current Time:  47ns
                WAIT FOR 34 ns;
                rst <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  53ns
                WAIT FOR 6 ns;
                CHECK_inst("00110101", 53);
                CHECK_pc("0001", 53);
                -- -------------------------------------
                -- -------------  Current Time:  67ns
                WAIT FOR 14 ns;
                clk_en <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  73ns
                WAIT FOR 6 ns;
                CHECK_inst("01000110", 73);
                CHECK_pc("0010", 73);
                CHECK_R1("10101", 73);
                -- -------------------------------------
                -- -------------  Current Time:  93ns
                WAIT FOR 20 ns;
                CHECK_inst("10000000", 93);
                CHECK_pc("0011", 93);
                CHECK_R2("00110", 93);
                -- -------------------------------------
                -- -------------  Current Time:  113ns
                WAIT FOR 20 ns;
                CHECK_inst("01100000", 113);
                CHECK_pc("0100", 113);
                CHECK_R3("00100", 113);
                -- -------------------------------------
                -- -------------  Current Time:  133ns
                WAIT FOR 20 ns;
                CHECK_inst("00100001", 133);
                CHECK_pc("0101", 133);
                CHECK_R2("00100", 133);
                -- -------------------------------------
                -- -------------  Current Time:  153ns
                WAIT FOR 20 ns;
                CHECK_inst("10000000", 153);
                CHECK_pc("0110", 153);
                CHECK_R1("00001", 153);
                -- -------------------------------------
                -- -------------  Current Time:  173ns
                WAIT FOR 20 ns;
                CHECK_inst("01100000", 173);
                CHECK_pc("0111", 173);
                CHECK_R3("00000", 173);
                -- -------------------------------------
                -- -------------  Current Time:  193ns
                WAIT FOR 20 ns;
                CHECK_inst("10110000", 193);
                CHECK_pc("1000", 193);
                CHECK_R2("00000", 193);
                -- -------------------------------------
                -- -------------  Current Time:  213ns
                WAIT FOR 20 ns;
                CHECK_inst("01100000", 213);
                CHECK_pc("1001", 213);
                CHECK_R1("00000", 213);
                -- -------------------------------------
                -- -------------  Current Time:  233ns
                WAIT FOR 20 ns;
                CHECK_inst("00100111", 233);
                CHECK_pc("1010", 233);
                -- -------------------------------------
                -- -------------  Current Time:  253ns
                WAIT FOR 20 ns;
                CHECK_inst("11000000", 253);
                CHECK_pc("1011", 253);
                CHECK_R1("00111", 253);
                -- -------------------------------------
                -- -------------  Current Time:  273ns
                WAIT FOR 20 ns;
                CHECK_GE_FLG('1', 273);
                CHECK_inst("11100000", 273);
                CHECK_pc("1100", 273);
                -- -------------------------------------
                -- -------------  Current Time:  293ns
                WAIT FOR 20 ns;
                CHECK_inst("00000000", 293);
                CHECK_pc("1101", 293);
                -- -------------------------------------
                -- -------------  Current Time:  313ns
                WAIT FOR 20 ns;
                CHECK_GE_FLG('0', 313);
                CHECK_pc("1110", 313);
                CHECK_R1("00000", 313);
                -- -------------------------------------
                -- -------------  Current Time:  333ns
                WAIT FOR 20 ns;
                CHECK_pc("1111", 333);
                -- -------------------------------------
                WAIT FOR 87 ns;

                IF (TX_ERROR = 0) THEN
                    STD.TEXTIO.write(TX_OUT, string'("No errors or warnings"));
                    ASSERT (FALSE) REPORT
                      "Simulation successful (not a failure).  No problems detected."
                      SEVERITY FAILURE;
                ELSE
                    STD.TEXTIO.write(TX_OUT, TX_ERROR);
                    STD.TEXTIO.write(TX_OUT,
                        string'(" errors found in simulation"));
                    ASSERT (FALSE) REPORT "Errors found during simulation"
                         SEVERITY FAILURE;
                END IF;
            END PROCESS;

    END testbench_arch;

