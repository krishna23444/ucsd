--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : ISE WebPACK
--  /   /         Filename : graycounter_tbw_crit.vhw
-- /___/   /\     Timestamp : Tue May 03 22:58:59 2005
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: graycounter_tbw_crit
--Device: Xilinx
--

library UNISIM;
use UNISIM.Vcomponents.ALL;
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY graycounter_tbw_crit IS
END graycounter_tbw_crit;

ARCHITECTURE testbench_arch OF graycounter_tbw_crit IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT graycounter
        PORT (
            CLK : In std_logic;
            CLK_CE : In std_logic;
            CLR : In std_logic;
            D : Out std_logic_vector (3 DownTo 0)
        );
    END COMPONENT;

    SIGNAL CLK : std_logic := '0';
    SIGNAL CLK_CE : std_logic := '1';
    SIGNAL CLR : std_logic := '1';
    SIGNAL D : std_logic_vector (3 DownTo 0) := "0000";

    SHARED VARIABLE TX_ERROR : INTEGER := 0;
    SHARED VARIABLE TX_OUT : LINE;

    BEGIN
        UUT : graycounter
        PORT MAP (
            CLK => CLK,
            CLK_CE => CLK_CE,
            CLR => CLR,
            D => D
        );

        PROCESS
            PROCEDURE CHECK_D(
                next_D : std_logic_vector (3 DownTo 0);
                TX_TIME : INTEGER
            ) IS
                VARIABLE TX_STR : String(1 to 4096);
                VARIABLE TX_LOC : LINE;
                BEGIN
                IF (D /= next_D) THEN
                    STD.TEXTIO.write(TX_LOC, string'("Error at time="));
                    STD.TEXTIO.write(TX_LOC, TX_TIME);
                    STD.TEXTIO.write(TX_LOC, string'("ns D="));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, D);
                    STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
                    IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_D);
                    STD.TEXTIO.write(TX_LOC, string'(" "));
                    TX_STR(TX_LOC.all'range) := TX_LOC.all;
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            BEGIN
                -- -------------  Current Time:  100ns
                WAIT FOR 100 ns;
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  120ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  130ns
                WAIT FOR 10 ns;
                CHECK_D("0010", 130);
                -- -------------------------------------
                -- -------------  Current Time:  160ns
                WAIT FOR 30 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  180ns
                WAIT FOR 20 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  200ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  220ns
                WAIT FOR 20 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  240ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  260ns
                WAIT FOR 20 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  280ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  300ns
                WAIT FOR 20 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  320ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  340ns
                WAIT FOR 20 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  360ns
                WAIT FOR 20 ns;
                CLK <= '0';
                WAIT FOR 640 ns;

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

