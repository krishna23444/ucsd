--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : ISE WebPACK
--  /   /         Filename : sixteenbit_adder_tbw.vhw
-- /___/   /\     Timestamp : Mon Apr 18 20:39:42 2005
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: sixteenbit_adder_tbw
--Device: Xilinx
--

library UNISIM;
use UNISIM.Vcomponents.ALL;
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY sixteenbit_adder_tbw IS
END sixteenbit_adder_tbw;

ARCHITECTURE testbench_arch OF sixteenbit_adder_tbw IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT sixteenbit_adder
        PORT (
            A : In std_logic_vector (15 DownTo 0);
            B : In std_logic_vector (15 DownTo 0);
            Ci : In std_logic;
            Co : Out std_logic;
            S : Out std_logic_vector (15 DownTo 0)
        );
    END COMPONENT;

    SIGNAL A : std_logic_vector (15 DownTo 0) := "0000000000000000";
    SIGNAL B : std_logic_vector (15 DownTo 0) := "0000000000000000";
    SIGNAL Ci : std_logic := '0';
    SIGNAL Co : std_logic := '0';
    SIGNAL S : std_logic_vector (15 DownTo 0) := "0000000000000000";

    SHARED VARIABLE TX_ERROR : INTEGER := 0;
    SHARED VARIABLE TX_OUT : LINE;

    BEGIN
        UUT : sixteenbit_adder
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
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            PROCEDURE CHECK_S(
                next_S : std_logic_vector (15 DownTo 0);
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
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            BEGIN
                -- -------------  Current Time:  50ns
                WAIT FOR 50 ns;
                CHECK_S("0010101001001101", 50);
                -- -------------------------------------
                -- -------------  Current Time:  100ns
                WAIT FOR 50 ns;
                A <= "0000000000000101";
                B <= "0010101001001000";
                -- -------------------------------------
                -- -------------  Current Time:  150ns
                WAIT FOR 50 ns;
                CHECK_Co('1', 150);
                CHECK_S("1101110011011111", 150);
                -- -------------------------------------
                -- -------------  Current Time:  200ns
                WAIT FOR 50 ns;
                A <= "1111000110010111";
                B <= "1110101101001000";
                -- -------------------------------------
                -- -------------  Current Time:  250ns
                WAIT FOR 50 ns;
                CHECK_S("0000001110001010", 250);
                -- -------------------------------------
                -- -------------  Current Time:  300ns
                WAIT FOR 50 ns;
                A <= "1010010111111110";
                B <= "0101110110001100";
                -- -------------------------------------
                -- -------------  Current Time:  350ns
                WAIT FOR 50 ns;
                CHECK_Co('0', 350);
                CHECK_S("1110101001010000", 350);
                -- -------------------------------------
                -- -------------  Current Time:  400ns
                WAIT FOR 50 ns;
                A <= "0101000010001010";
                B <= "1001100111000110";
                -- -------------------------------------
                -- -------------  Current Time:  450ns
                WAIT FOR 50 ns;
                CHECK_S("0101011000110010", 450);
                -- -------------------------------------
                -- -------------  Current Time:  500ns
                WAIT FOR 50 ns;
                A <= "0011100101001010";
                B <= "0001110011101000";
                -- -------------------------------------
                -- -------------  Current Time:  1000ns
                WAIT FOR 500 ns;
                Ci <= '0';

                IF (TX_ERROR = 0) THEN
                    STD.TEXTIO.write(TX_OUT, string'("No errors or warnings"));
                    STD.TEXTIO.writeline(RESULTS, TX_OUT);
                    ASSERT (FALSE) REPORT
                      "Simulation successful (not a failure).  No problems detected."
                      SEVERITY FAILURE;
                ELSE
                    STD.TEXTIO.write(TX_OUT, TX_ERROR);
                    STD.TEXTIO.write(TX_OUT,
                        string'(" errors found in simulation"));
                    STD.TEXTIO.writeline(RESULTS, TX_OUT);
                    ASSERT (FALSE) REPORT "Errors found during simulation"
                         SEVERITY FAILURE;
                END IF;
            END PROCESS;

    END testbench_arch;

