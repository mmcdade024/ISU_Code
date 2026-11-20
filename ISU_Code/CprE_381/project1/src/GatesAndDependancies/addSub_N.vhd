library IEEE;
use IEEE.std_logic_1164.all;


entity addSub_N is


	generic (N : integer := 16);
	port (
	
		i_A, i_B : in std_logic_vector(N-1 downto 0);
		n_Add_Sub : in std_logic;

		o_F : out std_logic_vector(N-1 downto 0);
		o_C : out std_logic -- carry out (not described but idk made sense)

	);

end addSub_N;


architecture structural of addSub_N is

	-- declarations

	component adder_N is
		generic(N : integer := 16);
		port (
			x0, x1 : in std_logic_vector(N-1 downto 0);
			cI : in std_logic;
			y0 : out std_logic_vector(N-1 downto 0);
			cO : out std_logic
		);
	end component;

	component invg_N is
		generic(N : integer := 16);
		port(
			x : in std_logic_vector(N-1 downto 0);
			y : out std_logic_vector(N-1 downto 0)
		);	
	end component;

	component mux2t1_N is
		generic(N : integer := 16); -- Generic of type integer for input/output data width. Default value is 32.
  		port(
			i_S          : in std_logic;
     		  	i_D0         : in std_logic_vector(N-1 downto 0);
      		 	i_D1         : in std_logic_vector(N-1 downto 0);
       			o_0          : out std_logic_vector(N-1 downto 0)
		);
	end component;

	-- signals 

	signal NOT_B : std_logic_vector(N-1 downto 0);
	signal s0 : std_logic_vector(N-1 downto 0);
	

begin

	inv0 : invg_N
		generic map(N=>N)
		port map(i_B, NOT_B);

	mux0 : mux2t1_N
		generic map(N=>N)
		port map(i_D0 => i_B, i_D1 => NOT_B , i_S => n_Add_Sub, o_0 => s0);

	add0 : adder_N
		generic map(N=>N)
		port map(x0 => i_A, x1 => s0, cI => n_Add_Sub, cO => o_C, y0 => o_F);

end structural;
