
-- One bit half adder
library IEEE;
use IEEE.std_logic_1164.all;

entity adder is 
	port(	
		x0, x1, cI : in std_logic;
		y0, cO : out std_logic
	);
end adder;


architecture structural of adder is

-- gates

component invg is
	port(
		i_A : in std_logic;
       		o_F : out std_logic
	);
end component;

component andg2 is 
	port(i_A          : in std_logic;
       i_B          : in std_logic;
       o_F          : out std_logic);
end component;

component org2 is
	port(i_A          : in std_logic;
       i_B          : in std_logic;
       o_F          : out std_logic);
end component;

component xorg2 is
	port(i_A          : in std_logic;
       i_B          : in std_logic;
       o_F          : out std_logic);
end component;

-- signals
signal NOTx0, NOTcI, NOTx1 : std_logic;
signal s0, s1, s2, s3 : std_logic;
signal s4, s5, s6, s7 : std_logic;

signal sa, sb, sc : std_logic;

begin
	---------------------------------------------------
	-- y0 logic
	inv0: invg
		port map(x0, NOTx0);

	xor0: xorg2
		port map(x1, cI, s0);

	and0: andg2
		port map(s0, NOTx0, s1);

	-- s1 is one input of the final or gate for y0	
	
	and1: andg2
		port map(x0, x1, s2);
	and2: andg2
		port map(s2, cI, s3);

	-- s3 is the other input of the final or gate for y0

	or0 : org2
		port map(s1, s3, sc);


	-- add missing logic from first implementation

	inv1: invg
		port map(x1, NOTx1);

	inv2: invg
		port map(cI, NOTcI);

	anda : andg2
		port map(NOTx1, NOTcI, sa);

	andb : andg2
		port map(sa, x0, sb);

	ora : org2
		port map(sc, sb, y0);

	
	


	---------------------------------------------------

	-- cO logic

	and3: andg2
		port map(x1, cI, s4);

	and4: andg2
		port map(x0, x1, s5);

	and5: andg2
		port map(x0, cI, s6);

	or1: org2
		port map(s4, s5, s7);

	or2: org2
		port map(s6, s7, cO);

	
	---------------------------------------------------



end structural;
