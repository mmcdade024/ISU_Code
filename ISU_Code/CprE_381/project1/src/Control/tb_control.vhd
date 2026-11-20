-- Test bench for control module

library IEEE;
use IEEE.std_logic_1164.all;


entity tb_control is
end tb_control;
architecture test of tb_control is

	component control is
		port(
        -- Control signals to the processesor

        -- Input
        opcode : in std_logic_vector(5 downto 0);
        func   : in std_logic_vector(5 downto 0); 

        -- Control flow signals
        jump : out std_logic; -- controls whether or not an alternative pc address is loaded
        bne : out std_logic;  -- if jump is also high, enable branch not equals
        beq : out std_logic;  -- if jump is also high, enable branch equals

        -- Register and memory signals
        reg_dst : out std_logic;
        reg_wr : out std_logic;
        mem_wr : out std_logic;

        -- compound signals
        reg_wr_src : out std_logic_vector(1 downto 0);
            -- Indicates what source is assigned to data on the register file.
            -- 0b00: ALU, 0b01: MEM, 0b10: LUI, 0b11: JAL 
        alu_op : out std_logic_vector(1 downto 0); -- not sure what size
        load_control : out std_logic_vector(1 downto 0) -- not sure what size
    );

	end component;
	
	signal s_opcode, s_func : std_logic_vector(5 downto 0);
	signal s_jump, s_bne, s_beq, s_reg_dst, s_reg_wr, s_mem_wr : std_logic;
	signal s_reg_wr_src, s_alu_op, s_load_control : std_logic_vector(1 downto 0);

begin

	DUT0 : control
		port map (
			opcode => s_opcode,
			func => s_func,
			jump => s_jump,
			bne => s_bne,
			beq => s_beq,
			reg_dst => s_reg_dst,
			reg_wr => s_reg_wr,
			mem_wr => s_mem_wr,
			reg_wr_src => s_reg_wr_src,
			alu_op => s_alu_op,
			load_control => s_load_control
		);

	TEST : process
	begin
	-- perform all r type instructions
	s_opcode <= b"000000";

	-- add
	s_func <= b"100000";
	wait for 10 ns;

	-- addu
	s_func <= b"100001";
	wait for 10 ns;

	-- and
	s_func <= b"100100";
	wait for 10 ns;

	-- jr
	s_func <= b"001000";
	wait for 10 ns;

	-- nor
	s_func <= b"100111";
	wait for 10 ns;

	-- or
	s_func <= b"100101";
	wait for 10 ns;

	-- xor
	s_func <= b"100110";
	wait for 10 ns;

	-- slt
	s_func <= b"101010";
	wait for 10 ns;

	-- sll
	s_func <= b"000000";
	wait for 10 ns;
	
	-- srl
	s_func <= b"000010";
	wait for 10 ns;

	-- sra
	s_func <= b"000011";
	wait for 10 ns;

	-- sub
	s_func <= b"100010";
	wait for 10 ns;

	-- subu
	s_func <= b"100011";
	wait for 10 ns;

	-- sllv
	s_func <= b"100100";
	wait for 10 ns;

	-- srlv
	s_func <= b"100110";
	wait for 10 ns;

	-- srav
	s_func <= b"100111";
	wait for 10 ns;

	-- perform other instructions
	s_func <= b"000000";

	-- addi
	s_opcode <= b"001000";
	wait for 10 ns;
	-- addiu
	s_opcode <= b"001001";
	wait for 10 ns;

	-- andi
	s_opcode <= b"001100";
	wait for 10 ns;
	-- lui
	s_opcode <= b"001111";
	wait for 10 ns;
	-- lw
	s_opcode <= b"100011";
	wait for 10 ns;
	-- xori
	s_opcode <= b"001110";
	wait for 10 ns;
	-- ori
	s_opcode <= b"001101";
	wait for 10 ns;
	-- slti
	s_opcode <= b"001010";
	wait for 10 ns;
	-- sw
	s_opcode <= b"101011";
	wait for 10 ns;
	-- beq
	s_opcode <= b"000100";
	wait for 10 ns;
	-- bne
	s_opcode <= b"000101";
	wait for 10 ns;
	-- j
	s_opcode <= b"000010";
	wait for 10 ns;
	-- jal
	s_opcode <= b"000011";
	wait for 10 ns;
	
	-- lb
	s_opcode <= b"100000";
	wait for 10 ns;
	-- lh
	s_opcode <= b"100001";
	wait for 10 ns;
	-- lbu
	s_opcode <= b"100100";
	wait for 10 ns;
	-- lhu
	s_opcode <= b"100101";
	wait for 10 ns;	


	end process;

end test;
