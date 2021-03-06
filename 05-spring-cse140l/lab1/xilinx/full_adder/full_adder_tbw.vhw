--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : ISE WebPACK
--  /   /         Filename : full_adder_tbw.vhw
-- /___/   /\     Timestamp : Mon Apr 18 18:03:01 2005
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: full_adder_tbw
--Device: Xilinx
--

library UNISIM;
use UNISIM.Vcomponents.ALL;
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY full_adder_tbw IS
END full_adder_tbw;

ARCHITECTURE testbench_arch OF full_adder_tbw IS
    COMPONENT full_adder
        PORT (
            A : In std_logic;
            B : In std_logic;
            Ci : In std_logic;
            Co : Out std_logic;
            S : Out std_logic
        );
    END COMPONENT;

    SIGNAL A : std_logic := '0';
    SIGNAL B : std_logic := '0';
    SIGNAL Ci : std_logic := '0';
    SIGNAL Co : std_logic := '0';
    SIGNAL S : std_logic := '0';

    SHARED VARIABLE TX_ERROR : INTEGER := 0;
    SHARED VARIABLE TX_OUT : LINE;

    BEGIN
        UUT : full_adder
        PORT MAP (
            A => A,
            B => B,
            Ci => Ci,
            Co => Co,
            S => S
        );

        PROCESS
            PROCEDURE CHECK_Co(
                next_Co : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (Co /= next_Co) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns Co="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, Co);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_Co);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_S(
                next_S : std_logic;
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (S /= next_S) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns S="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, S);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_S);
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
                CHECK_S('1', 50);
                -- -------------------------------------
                -- -------------  Current Time:  100ns
                WAIT FOR 50 ns;
                A <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  200ns
                WAIT FOR 100 ns;
                A <= '0';
                B <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  250ns
                WAIT FOR 50 ns;
                CHECK_Co('1', 250);
                CHECK_S('0', 250);
                -- -------------------------------------
                -- -------------  Current Time:  300ns
                WAIT FOR 50 ns;
                A <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  350ns
                WAIT FOR 50 ns;
                CHECK_Co('0', 350);
                CHECK_S('1', 350);
                -- -------------------------------------
                -- -------------  Current Time:  400ns
                WAIT FOR 50 ns;
                A <= '0';
                B <= '0';
                Ci <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  450ns
                WAIT FOR 50 ns;
                CHECK_Co('1', 450);
                CHECK_S('0', 450);
                -- -------------------------------------
                -- -------------  Current Time:  500ns
                WAIT FOR 50 ns;
                A <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  600ns
                WAIT FOR 100 ns;
                A <= '0';
                B <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  650ns
                WAIT FOR 50 ns;
                CHECK_S('1', 650);
                -- -------------------------------------
                -- -------------  Current Time:  700ns
                WAIT FOR 50 ns;
                A <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  750ns
                WAIT FOR 50 ns;
                CHECK_Co('0', 750);
                -- -------------------------------------
                -- -------------  Current Time:  800ns
                WAIT FOR 50 ns;
                A <= '0';
                B <= '0';
                WAIT FOR 200 ns;

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

