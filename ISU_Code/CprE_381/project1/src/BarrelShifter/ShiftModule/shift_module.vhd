-- submodule of the barrel shifter 
-- sammc

library IEEE;
use IEEE.std_logic_1164.all;

entity shift_module is

	generic (SHAMT : integer := 16);
	port (
		 -- '0' -> srl, '1' -> sra
        i_arithmetic : in std_logic;
        i_shift : in std_logic;
        i_X : in std_logic_vector(31 downto 0);
        o_Y : out std_logic_vector(31 downto 0)
	);

end shift_module;


architecture structural of shift_module is

	component mux2t1_N is
		generic(N : integer := 16); -- Generic of type integer for input/output data width. Default value is 32.
  		port(
			i_S          : in std_logic;
       		i_D0         : in std_logic_vector(N-1 downto 0);
       		i_D1         : in std_logic_vector(N-1 downto 0);
       		o_0          : out std_logic_vector(N-1 downto 0)
  		);
	end component; 

	-- signals that represent the final output in srl or sra mode
	signal s_srl : std_logic_vector(31 downto 0);
	signal s_sra : std_logic_vector(31 downto 0);

	signal low   : std_logic_vector(31 downto 0);
	-- signals that are used to perform the respectives operations
	signal t_srl : std_logic_vector(31 downto 0);
	signal t_sra : std_logic_vector(31 downto 0);

begin

	low <= x"00000000";
	
	-- signals that perform srl
	t_srl(31 downto 31-SHAMT) <= low(31 downto 31-SHAMT);
	t_srl(31-(SHAMT) downto 0) <= i_X(31 downto SHAMT);

	-- toggle shift for srl
	MUX_SRL : mux2t1_N
		generic map (N => 32)
		port map (
			i_S => i_shift,
			i_D0 => i_X, 									-- argument 1 is the normal case of no bit shift
			i_D1 => t_srl, 		-- argument 2 is the N msb in the N lsb of the target, or the properly bitshifted result
			o_0 => s_srl									-- output into s_srl the complete signal for the operation of srl
		);
	
	-- Signal sthat performs sra
	t_sra(31 downto (31-(SHAMT-1))) <= i_X(SHAMT-1 downto 0);
	t_sra(31-SHAMT downto 0) <= i_X(31 downto SHAMT);

	-- toggle shift for sra
	MUX_SRA : mux2t1_N
		generic map (N => 32)
		port map (
			i_S => i_shift,
			i_D0 => i_X,			-- argument 1 is the normal case of no bit shift
			i_D1 => t_sra,			-- argument 2's bits n downto 0 is the N msb from the input signal
			o_0 => s_sra			-- output into s_sra the complete signal for the operation of sra
		);

	-- Select which signal to use, srl or sra
	MUX_ARITHMETIC : mux2t1_N
		generic map (N => 32)
		port map (
			i_S => i_arithmetic,
			i_D0 => s_srl,
			i_D1 => s_sra,
			o_0 => o_Y
		);

end structural;


		


