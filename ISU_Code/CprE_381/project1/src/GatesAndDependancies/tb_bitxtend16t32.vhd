library IEEE;
use IEEE.std_logic_1164.all;

entity tb_bitxtend16t32 is

end tb_bitxtend16t32;

architecture test of tb_bitxtend16t32 is

	component bitxtend16t32 is
		port(
			n_Sign: in 	std_logic; -- when high, sign extend, when low zero extend
			i_Imm : in 	std_logic_vector(15 downto 0);
			o_O   : out 	std_logic_vector(31 downto 0)
		);
	end component;

	signal i : std_logic_vector(15 downto 0);
	signal o : std_logic_vector(31 downto 0);
	signal n : std_logic;

begin

	DUT0 : bitxtend16t32
		port map (
			n_Sign => n,
			i_Imm  => i,
			o_O    => o
		);

	TEST : process
	begin

		i <= x"0000";
		n <= '0';
		wait for 10 ns;

		i <= x"F000";
		n <= '1';
		wait for 10 ns;

		i <= x"0FFF";
		n <= '1';
		wait for 10 ns;

		i <= x"F000";
		n <= '0';
		wait for 10 ns;

		i <= x"AAAA";
		n <= '1';
		wait for 10 ns;

		
	
	end process;


	


end test;
