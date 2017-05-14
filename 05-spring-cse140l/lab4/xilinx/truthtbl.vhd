--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    23:04:23 05/31/05
-- Design Name:    
-- Module Name:    truthtbl - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity truthtbl is
	PORT (D : IN STD_LOGIC_VECTOR(7 DOWNTO 3);
			clr, R1_en, R2_en, R2_sel, R3_en, ov_en, ge_en : OUT STD_LOGIC;
			sel, shf_of : OUT STD_LOGIC_VECTOR(1 DOWNTO 0));
end truthtbl;

architecture Behavioral of truthtbl is

begin
	process(D)
	begin
		if(D(7 downto 5) = "000") then
			clr <= '1';
			R1_en <= '0';
			R2_en <= '0';
			R2_sel <= '0';
			R3_en <= '0';
			ov_en <= '0';
			ge_en <= '0';
			sel(1 downto 0) <= "00";
			shf_of(1 downto 0) <= "00";
		end if;
		if(D(7 downto 5) = "001") then
			clr <= '0';
			R1_en <= '1';
			R2_en <= '0';
			R2_sel <= '0';
			R3_en <= '0';
			ov_en <= '0';
			ge_en <= '0';
			sel(1 downto 0) <= "00";
			shf_of(1 downto 0) <= "00";
		end if;
		if(D(7 downto 5) = "010") then
			clr <= '0';
			R1_en <= '0';
			R2_en <= '1';
			R2_sel <= '0';
			R3_en <= '0';
			ov_en <= '0';
			ge_en <= '0';
			sel(1 downto 0) <= "00";
			shf_of(1 downto 0) <= "00";
		end if;
		if(D(7 downto 5) = "011") then
			clr <= '0';
			R1_en <= '0';
			R2_en <= '1';
			R2_sel <= '1';
			R3_en <= '0';
			ov_en <= '0';
			ge_en <= '0';
			sel(1 downto 0) <= "00";
			shf_of(1 downto 0) <= "00";
		end if;
		if(D(7 downto 5) = "100") then
			clr <= '0';
			R1_en <= '0';
			R2_en <= '0';
			R2_sel <= '0';
			R3_en <= '1';
			ov_en <= '1';
			ge_en <= '0';
			sel(1 downto 0) <= "01";
			shf_of(1 downto 0) <= "00";
		end if;
		if(D(7 downto 5) = "101") then
			clr <= '0';
			R1_en <= '0';
			R2_en <= '0';
			R2_sel <= '0';
			R3_en <= '1';
			ov_en <= '0';
			ge_en <= '0';
			sel(1 downto 0) <= "00";
			shf_of(1 downto 0) <= D(4 downto 3);
		end if;
		if(D(7 downto 5) = "110") then
			clr <= '0';
			R1_en <= '0';
			R2_en <= '0';
			R2_sel <= '0';
			R3_en <= '0';
			ov_en <= '0';
			ge_en <= '1';
			sel(1 downto 0) <= "11";
			shf_of(1 downto 0) <= "00";
		end if;
		if(D(7 downto 5) = "111") then
			clr <= '0';
			R1_en <= '0';
			R2_en <= '0';
			R2_sel <= '0';
			R3_en <= '1';
			ov_en <= '0';
			ge_en <= '0';
			sel(1 downto 0) <= "10";
			shf_of(1 downto 0) <= "00";
		end if;
	end process;
end Behavioral;