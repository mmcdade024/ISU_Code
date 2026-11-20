-- ripple carry adder

library IEEE;
use IEEE.std_logic_1164.all;

entity adder_N is

	generic(N : integer := 16);
	port (
		x0, x1 : in std_logic_vector(N-1 downto 0);
		cI : in std_logic;
		y0 : out std_logic_vector(N-1 downto 0);
		cO : out std_logic
	);

end adder_N;


architecture structural of adder_N is

	component adder is
		port(	
			x0, x1, cI : in std_logic;
			y0, cO : out std_logic
		);
	end component;

	-- create an array to make storing carry values between ripples easier. one extra bit than datawidth to store the output carry
	signal cArr : std_logic_vector(N downto 0);

begin
	-- add the carry in bit to the start of the array of carry values
	cArr(0) <= cI;

	G_adders: for i in 0 to N-1 generate
		adderI : adder
			port map(
				x0 => x0(i),
				x1 => x1(i),
				cI => cArr(i),
				y0 => y0(i),
				cO => cArr(i+1) -- now it will be assigned as the carry in for the next generation
			);
	end generate G_adders;


	-- send the last carryout out
	cO <= cArr(N);





end structural;


