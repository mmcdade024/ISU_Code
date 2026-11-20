

-- 2 to 1 mux using provided logic gates
-- when select line is low, output is the same as i0
-- when select line is high, output is the same as i1

library IEEE;
use IEEE.std_logic_1164.all;

entity mux2t1 is
	port ( 
		i_D0, i_D1, i_S	: in std_logic; 
		o_0 		: out std_logic
	);
end mux2t1;

architecture structural of mux2t1 is

signal s0, s1, s2 : std_logic;


component andg2
	port(
		i_A, i_B     : in std_logic;
       		o_F          : out std_logic
	);
end component;

component invg
	port(
		i_A          : in std_logic;
		o_F          : out std_logic
	);
end component;

component org2
	port(
		i_A          : in std_logic;
       		i_B          : in std_logic;
       		o_F          : out std_logic
	);
end component;

begin

inv0 : invg
	port map (
		i_A =>i_S,
		o_F => s0
	);

and0 : andg2
	port map (
		i_A => i_D0,
		i_B => s0,
		o_F => s1
	);	

and1 : andg2
	port map (
		i_A => i_D1,
		i_B => i_S,
		o_F => s2
	);

or0 : org2
	port map (
		i_A => s1,
		i_B => s2,
		o_F => o_0
	);


end structural;


