-- 16 to 32 bit extender with a control bit to switch between sign extended and zero extended.

library IEEE;
use IEEE.std_logic_1164.all;

entity bitxtend16t32 is
	port(
		n_Sign: in 	std_logic; -- when high, sign extend, when low zero extend
		i_Imm : in 	std_logic_vector(15 downto 0);
		o_O   : out 	std_logic_vector(31 downto 0)
	);
end bitxtend16t32;

architecture dataflow of bitxtend16t32 is


begin 

	o_O(15 downto 0)  <= i_Imm;
	o_O(31 downto 16) <= x"FFFF" when ((n_Sign = '1') and (i_Imm(15) = '1')) else x"0000";
	
	
end dataflow;
