-- test for the shift_module submodule used in the barrel_shifter module

library IEEE;
use IEEE.std_logic_1164.all;

entity tb_shift_module is

end tb_shift_module;

architecture test of tb_shift_module is

	component shift_module is
		generic (SHAMT : integer := 16);
		port (
			 -- '0' -> srl, '1' -> sra
	        i_arithmetic : in std_logic;
	        i_shift : in std_logic;
	        i_X : in std_logic_vector(31 downto 0);
	        o_Y : out std_logic_vector(31 downto 0)
		);
	end component;

	signal s_arithemetic, s_shift : std_logic;
	signal s_X, s_Y : std_logic_vector(31 downto 0);

begin

	DUT0 : shift_module
		generic map (SHAMT => 16)
		port map (
			i_arithmetic => s_arithemetic,
			i_shift => s_shift,
			i_X => s_X,
			o_Y => s_Y
		);

	TEST : process
	begin

		-- Test Value
		s_X <= x"10100001";

		-- test shift right logical
		s_arithemetic <= '0';

		s_shift <= '0';
		wait for 10 ns;

		s_shift <= '1';
		wait for 10 ns;

		-- test shift right arithmetic
		s_arithemetic <= '1';

		s_shift <= '0';
		wait for 10 ns;

		s_shift <= '1';
		wait for 10 ns;
	

	end process;

	



end test;
