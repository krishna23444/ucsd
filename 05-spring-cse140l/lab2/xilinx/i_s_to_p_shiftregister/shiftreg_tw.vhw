--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : ISE WebPACK
--  /   /         Filename : shiftreg_tw.vhw
-- /___/   /\     Timestamp : Tue May 03 23:53:05 2005
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: shiftreg_tw
--Device: Xilinx
--

library UNISIM;
use UNISIM.Vcomponents.ALL;
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY shiftreg_tw IS
END shiftreg_tw;

ARCHITECTURE testbench_arch OF shiftreg_tw IS
    COMPONENT shiftreg
        PORT (
            CLK : In std_logic;
            CLK_EN : In std_logic;
            CLR : In std_logic;
            Din : In std_logic;
            D0 : Out std_logic;
            D1 : Out std_logic;
            D2 : Out std_logic;
            D3 : Out std_logic
        );
    END COMPONENT;

    SIGNAL CLK : std_logic := '0';
    SIGNAL CLK_EN : std_logic := '0';
    SIGNAL CLR : std_logic := '1';
    SIGNAL Din : std_logic := '0';
    SIGNAL D0 : std_logic := '0';
    SIGNAL D1 : std_logic := '0';
    SIGNAL D2 : std_logic := '0';
    SIGNAL D3 : std_logic := '0';

    SHARED VARIABLE TX_ERROR : INTEGER := 0;
    SHARED VARIABLE TX_OUT : LINE;

    BEGIN
        UUT : shiftreg
        PORT MAP (
            CLK => CLK,
            CLK_EN => CLK_EN,
            CLR => CLR,
            Din => Din,
            D0 => D0,
            D1 => D1,
            D2 => D2,
            D3 => D3
        );

        PROCESS
            PROCEDURE CHECK_D0(
                next_D0 : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (D0 /= next_D0) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns D0="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, D0);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_D0);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_D1(
                next_D1 : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (D1 /= next_D1) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns D1="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, D1);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_D1);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_D2(
                next_D2 : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (D2 /= next_D2) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns D2="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, D2);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_D2);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_D3(
                next_D3 : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (D3 /= next_D3) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns D3="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, D3);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_D3);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            BEGIN
                -- -------------  Current Time:  50ns
                WAIT FOR 50 ns;
                CLK_EN <= '1';
                CLR <= '0';
                Din <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  100ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  125ns
                WAIT FOR 25 ns;
                CHECK_D0('1', 125);
                -- -------------------------------------
                -- -------------  Current Time:  150ns
                WAIT FOR 25 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  200ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  225ns
                WAIT FOR 25 ns;
                CHECK_D1('1', 225);
                -- -------------------------------------
                -- -------------  Current Time:  250ns
                WAIT FOR 25 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  300ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  325ns
                WAIT FOR 25 ns;
                CHECK_D2('1', 325);
                -- -------------------------------------
                -- -------------  Current Time:  350ns
                WAIT FOR 25 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  400ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  425ns
                WAIT FOR 25 ns;
                CHECK_D3('1', 425);
                -- -------------------------------------
                -- -------------  Current Time:  450ns
                WAIT FOR 25 ns;
                CLK <= '1';
                Din <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  500ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  525ns
                WAIT FOR 25 ns;
                CHECK_D0('0', 525);
                -- -------------------------------------
                -- -------------  Current Time:  550ns
                WAIT FOR 25 ns;
                CLK <= '1';
                Din <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  600ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  625ns
                WAIT FOR 25 ns;
                CHECK_D1('0', 625);
                -- -------------------------------------
                -- -------------  Current Time:  650ns
                WAIT FOR 25 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  700ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  725ns
                WAIT FOR 25 ns;
                CHECK_D2('0', 725);
                -- -------------------------------------
                -- -------------  Current Time:  750ns
                WAIT FOR 25 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  800ns
                WAIT FOR 50 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  825ns
                WAIT FOR 25 ns;
                CHECK_D3('0', 825);
                -- -------------------------------------
                -- -------------  Current Time:  850ns
                WAIT FOR 25 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  900ns
                WAIT FOR 50 ns;
                CLK <= '0';
                WAIT FOR 100 ns;

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

