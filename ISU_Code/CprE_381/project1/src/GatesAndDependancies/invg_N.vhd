-- One's complimenter / n-bit inverter

library IEEE;
use IEEE.std_logic_1164.all;

entity invg_N is 
	generic(N : integer := 16);
	port(
		x : in std_logic_vector(N-1 downto 0);
		y : out std_logic_vector(N-1 downto 0)
	);
end invg_N;

architecture structural of invg_N is

component invg is
	port(
		i_A : in std_logic;
       		o_F : out std_logic
	);
end component;

begin


generateInverters: for i in 0 to N-1 generate
	inverterI : invg
		port map(
			i_A => x(i),
			o_F => y(i)
		);
end generate generateInverters;


end structural;
