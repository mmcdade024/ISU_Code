-- Test bench for the barrel shifter to make sure it works correctly

library IEEE;
use IEEE.std_logic_1164.all;

entity tb_barrel_shifter is
end entity tb_barrel_shifter;

architecture test of tb_barrel_shifter is

	component barrel_shifter is
		port (
        	-- '00' -> sll(v), '01' -> srl(v), '10' -> sra(v)
        	i_mode : in std_logic_vector(1 downto 0);
        	i_shamt : in std_logic_vector(4 downto 0);
        	i_X : in std_logic_vector(31 downto 0);
        	o_Y : out std_logic_vector(31 downto 0)
		);
	end component;

	signal s_mode : std_logic_vector(1 downto 0);
	signal s_shamt : std_logic_vector(4 downto 0);
	signal s_X, s_Y : std_logic_vector(31 downto 0);

begin

	DUT0 : barrel_shifter
		port map (
			i_mode => s_mode,
			i_shamt => s_shamt,
			i_X => s_X,
			o_Y => s_Y
		);

	TEST : process
	begin

	-- SLL
	s_mode <= b"00";

	s_shamt <= b"00000";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;

	s_shamt <= b"00001";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;

	s_shamt <= b"11111";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;


	-- SRL
	s_mode <= b"01";

	s_shamt <= b"00000";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;

	s_shamt <= b"01010";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;
	
	s_shamt <= b"00101";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;


	-- SRA
	s_mode <= b"10";

	s_shamt <= b"00000";
	s_X <= x"AAAAAAAA";
	wait for 10 ns;

	s_shamt <= b"00101";
	s_X <= x"AAAAFFFF";
	wait for 10 ns;

	s_shamt <= b"11000";
	s_X <= x"FFFFAAAA";
	wait for 10 ns;



	end process;




end test;
