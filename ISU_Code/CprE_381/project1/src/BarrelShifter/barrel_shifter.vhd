-- Full Barrel Shifter Implementation
-- sammc

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity barrel_shifter is

	port (
		-- 0b00 -> srl, 0b01 -> sra, 0b10 -> sll
		i_mode : std_logic_vector(1 downto 0);
		i_shamt : std_logic_vector(4 downto 0);
		i_X : in std_logic_vector(31 downto 0);
		o_Y : out std_logic_vector(31 downto 0)
	);

end barrel_shifter;

architecture structural of barrel_shifter is

	component right_barrel_shifter is
		port (
		    i_art: in std_logic;-- '0' -> srl, '1' -> sra
        	i_shamt : in std_logic_vector(4 downto 0);
        	i_X : in std_logic_vector(31 downto 0);
        	o_Y : out std_logic_vector(31 downto 0)
		);
	end component;

	component invg_N is
		generic(N : integer := 16);
		port (
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

	signal i_nX, s_nY : std_logic_vector(31 downto 0);

	signal s_X, s_Y : std_logic_vector(31 downto 0);

begin

	rbs : right_barrel_shifter
		port map(
			i_art => i_mode(0),
			i_shamt => i_shamt,
			i_X => s_X,
			o_Y => s_Y
		);

	-- optionally invert the input before it passes into the barrel shifter for left shifting
	invX : invg_N
		generic map (N => 32)
		port map (x => i_X, y => i_nX);

	-- select whether to right shift or left shift, inverting the input and output for the latter
	muxX : mux2t1_N
		generic map(N => 32)
		port map (i_S => i_mode(1), i_D0 => i_X, i_D1 => i_nX, o_0 => s_X);
	
	-- optionally invert the output for sll
	invY : invg_N
		generic map (N => 32)
		port map (x => s_Y, y => s_nY);

	-- select the output, when using srl/sra or sll
	muxY : mux2t1_N
		generic map(N => 32)
		port map (i_S => i_mode(1), i_D0 => s_Y, i_D1 => s_nY, o_0 => o_Y);
	



	


end structural;
