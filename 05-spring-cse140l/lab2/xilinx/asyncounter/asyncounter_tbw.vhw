--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 7.1.01i
--  \   \         Application : ISE WebPACK
--  /   /         Filename : asyncounter_tbw.vhw
-- /___/   /\     Timestamp : Tue May 03 02:51:15 2005
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: asyncounter_tbw
--Device: Xilinx
--

library UNISIM;
use UNISIM.Vcomponents.ALL;
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY asyncounter_tbw IS
END asyncounter_tbw;

ARCHITECTURE testbench_arch OF asyncounter_tbw IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT asyncounter
        PORT (
            CLK : In std_logic;
            CLK_CE : In std_logic;
            CLR : In std_logic;
            VCC : In std_logic;
            D0 : Out std_logic;
            D1 : Out std_logic;
            D2 : Out std_logic;
            D3 : Out std_logic
        );
    END COMPONENT;

    SIGNAL CLK : std_logic := '0';
    SIGNAL CLK_CE : std_logic := '0';
    SIGNAL CLR : std_logic := '0';
    SIGNAL VCC : std_logic := '1';
    SIGNAL D0 : std_logic := '0';
    SIGNAL D1 : std_logic := '0';
    SIGNAL D2 : std_logic := '0';
    SIGNAL D3 : std_logic := '0';

    SHARED VARIABLE TX_ERROR : INTEGER := 0;
    SHARED VARIABLE TX_OUT : LINE;

    BEGIN
        UUT : asyncounter
        PORT MAP (
            CLK => CLK,
            CLK_CE => CLK_CE,
            CLR => CLR,
            VCC => VCC,
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
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
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
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
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
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
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
                    STD.TEXTIO.writeline(RESULTS, TX_LOC);
                    STD.TEXTIO.Deallocate(TX_LOC);
                    ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
                    TX_ERROR := TX_ERROR + 1;
                END IF;
            END;
            BEGIN
                -- -------------  Current Time:  10ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 10);
                -- -------------------------------------
                -- -------------  Current Time:  20ns
                WAIT FOR 10 ns;
                CLK <= '1';
                CLK_CE <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  40ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  50ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 50);
                CHECK_D1('1', 50);
                -- -------------------------------------
                -- -------------  Current Time:  60ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  80ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  90ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 90);
                -- -------------------------------------
                -- -------------  Current Time:  100ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  120ns
                WAIT FOR 20 ns;
                CLK <= '0';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  130ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 130);
                CHECK_D1('0', 130);
                CHECK_D2('1', 130);
                -- -------------------------------------
                -- -------------  Current Time:  140ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  160ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  170ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 170);
                -- -------------------------------------
                -- -------------  Current Time:  180ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  200ns
                WAIT FOR 20 ns;
                CLK <= '0';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  210ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 210);
                CHECK_D1('1', 210);
                -- -------------------------------------
                -- -------------  Current Time:  220ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  240ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  250ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 250);
                -- -------------------------------------
                -- -------------  Current Time:  260ns
                WAIT FOR 10 ns;
                CLK <= '1';
                VCC <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  280ns
                WAIT FOR 20 ns;
                CLK <= '0';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  290ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 290);
                CHECK_D1('0', 290);
                CHECK_D2('0', 290);
                CHECK_D3('1', 290);
                -- -------------------------------------
                -- -------------  Current Time:  300ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  320ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  330ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 330);
                -- -------------------------------------
                -- -------------  Current Time:  340ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  360ns
                WAIT FOR 20 ns;
                CLK <= '0';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  370ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 370);
                CHECK_D1('1', 370);
                -- -------------------------------------
                -- -------------  Current Time:  380ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  400ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  410ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 410);
                -- -------------------------------------
                -- -------------  Current Time:  420ns
                WAIT FOR 10 ns;
                CLK <= '1';
                VCC <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  440ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  450ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 450);
                CHECK_D1('0', 450);
                CHECK_D2('1', 450);
                -- -------------------------------------
                -- -------------  Current Time:  460ns
                WAIT FOR 10 ns;
                CLK <= '1';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  480ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  490ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 490);
                -- -------------------------------------
                -- -------------  Current Time:  500ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  520ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  530ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 530);
                CHECK_D1('1', 530);
                -- -------------------------------------
                -- -------------  Current Time:  540ns
                WAIT FOR 10 ns;
                CLK <= '1';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  560ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  570ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 570);
                -- -------------------------------------
                -- -------------  Current Time:  580ns
                WAIT FOR 10 ns;
                CLK <= '1';
                VCC <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  600ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  610ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 610);
                CHECK_D1('0', 610);
                CHECK_D2('0', 610);
                CHECK_D3('0', 610);
                -- -------------------------------------
                -- -------------  Current Time:  620ns
                WAIT FOR 10 ns;
                CLK <= '1';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  640ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  650ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 650);
                -- -------------------------------------
                -- -------------  Current Time:  660ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  680ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  690ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 690);
                CHECK_D1('1', 690);
                -- -------------------------------------
                -- -------------  Current Time:  700ns
                WAIT FOR 10 ns;
                CLK <= '1';
                CLR <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  720ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  730ns
                WAIT FOR 10 ns;
                CHECK_D0('1', 730);
                -- -------------------------------------
                -- -------------  Current Time:  740ns
                WAIT FOR 10 ns;
                CLK <= '1';
                VCC <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  760ns
                WAIT FOR 20 ns;
                CLK <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  770ns
                WAIT FOR 10 ns;
                CHECK_D0('0', 770);
                CHECK_D1('0', 770);
                CHECK_D2('1', 770);
                -- -------------------------------------
                -- -------------  Current Time:  780ns
                WAIT FOR 10 ns;
                CLK <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  800ns
                WAIT FOR 20 ns;
                CLK <= '0';
                WAIT FOR 200 ns;

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

