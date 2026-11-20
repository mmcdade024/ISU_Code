-- testbench for addSub_N.vhd


library IEEE;
use IEEE.std_logic_1164.all;


entity tb_addSub_N is

	generic (width : integer := 16);

end tb_addSub_N;


architecture testbench of tb_addSub_N is
	
	component addSub_N is

		generic (N : integer := 16);
		port (
	
			i_A, i_B : in std_logic_vector(N-1 downto 0);
			n_Add_Sub : in std_logic;

			o_F : out std_logic_vector(N-1 downto 0);
			o_C : out std_logic -- carry out (not described but idk made sense)

		);


	end component;


	signal x0, x1, y0 : std_logic_vector(width-1 downto 0);
	signal controlBit, carryOut : std_logic;

begin

	DUT0 : addSub_N
		generic map (N => width)
		port map (
			i_A => x0,
			i_B => x1,
			n_Add_Sub => controlBit,
			
			o_F => y0,
			o_C => carryOut
		);


	tests : process
	begin


	-- test set 1 -- addition

	x0 <= x"FFF0";
	x1 <= x"000F";
	controlBit <= '0';
	wait for 10 ns;

	x0 <= x"F0F0";
	x1 <= x"0F0F";
	controlBit <= '0';
	wait for 10 ns;

	x0 <= x"AAAA";
	x1 <= x"1111";
	controlBit <= '0';
	wait for 10 ns;

	x0 <= x"2222";
	x1 <= x"3333";
	controlBit <= '0';
	wait for 10 ns;

	-- test set 2 -- subtraction

	x0 <= x"FFFF";
	x1 <= x"0F0F";
	controlBit <= '1';
	wait for 10 ns;

	-- negative result
	x0 <= x"AAAA";
	x1 <= x"BBBB";
	controlBit <= '1';
	wait for 10 ns;

	x0 <= x"0FF0";
	x1 <= x"00A5";
	controlBit <= '1';
	wait for 10 ns;

	x0 <= x"DDDD";
	x1 <= x"3333";
	controlBit <= '1';
	wait for 10 ns;

	-- test set 3 -- inversion and overflow

	x0 <= x"FFFB";
	x1 <= x"000F";
	controlBit <= '0';
	wait for 10 ns;

	x0 <= x"0000";
	x1 <= x"0005";
	controlBit <= '1';
	wait for 10 ns;

	x0 <= x"0000";
	x1 <= x"FFFF";
	controlBit <= '1';
	wait for 10 ns;

	x0 <= x"F00D";
	x1 <= x"0000";
	controlBit <= '1';
	wait for 10 ns;



	end process;


end testbench;
