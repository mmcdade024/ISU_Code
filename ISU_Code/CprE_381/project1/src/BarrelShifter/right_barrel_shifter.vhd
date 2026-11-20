-- Right Only Barrel Shifter Implementation
-- sammc

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity right_barrel_shifter is

    port (

        -- '0' -> srl, '1' -> sra
        i_art : in std_logic;
        i_shamt : in std_logic_vector(4 downto 0);
        i_X : in std_logic_vector(31 downto 0);
        o_Y : out std_logic_vector(31 downto 0)

    );


end entity right_barrel_shifter;
architecture structural of right_barrel_shifter is
	component shift_module is
		generic (SHAMT : integer := 16);
		port (
        	i_arithmetic : in std_logic; -- '0' -> srl, '1' -> sra
        	i_shift : in std_logic;
        	i_X : in std_logic_vector(31 downto 0);
       		o_Y : out std_logic_vector(31 downto 0)
		);
	end component;
  	signal s_0, s_1, s_2, s_3 : std_logic_vector(31 downto 0);
begin

	shift_16 : shift_module
		generic map (16)
		port map (
			i_arithmetic => i_art,
			i_shift => i_shamt(4),
			i_X => i_X,
			o_Y => s_0
		);
    
	shift_8 : shift_module
		generic map(8)
		port map (
			i_arithmetic => i_art,
			i_shift => i_shamt(3),
			i_X => s_0,
			o_Y => s_1
		);

	shift_4 : shift_module
		generic map(4)
		port map (
			i_arithmetic => i_art,
			i_shift => i_shamt(2),
			i_X => s_1,
			o_Y => s_2
		);

	shift_2 : shift_module
		generic map(2)
		port map (
			i_arithmetic => i_art,
			i_shift => i_shamt(1),
			i_X => s_2,
			o_Y => s_3
		);

	shift_1 : shift_module
		generic map(1)
		port map (
			i_arithmetic => i_art,
			i_shift => i_shamt(0),
			i_X => s_3,
			o_Y => o_Y
		);

end structural;
