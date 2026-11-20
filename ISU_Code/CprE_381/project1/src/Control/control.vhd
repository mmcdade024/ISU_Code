-- control logic implementation

library IEEE;
use IEEE.std_logic_1164.all;

entity Control is

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

end Control;

architecture dataflow of Control is
begin

    -- Control Flow Signals
    jump <= '1' when ((opcode(5 downto 3) = b"000") and (func = b"001000" or opcode(2 downto 0) /= b"000"));
        -- jump = op[5:3] == 0 & (func = 0x08 || op[2:1] != 0x00)
    bne <= '1' when (opcode = b"000101") else '0';
        -- bne = op == 5
    beq <= '1' when (opcode = b"000100") else '0';
        -- beq = op == 4

    -- General Controls
    reg_dst <= '1' when (opcode = b"000000") else '0';  
        -- reg_dst = opcode == 0
    reg_wr  <= '0' when (opcode(5 downto 1) = b"00010" or opcode = b"000010" or (opcode = b"000000" and func = b"001000")) else '1';
        -- reg_wr = (opcode is 2, 4 or 5) or (opcode is 0 and func is 8)
    mem_wr  <= '1' when (opcode(5) = '1' and opcode(3) = '0'); 
        -- mem_wr = 0b1x.0xxx

    -- Control the source of the data to registers
    reg_wr_src <= b"11" when (opcode = b"000011") else     -- jal
                  b"10" when (opcode = b"000001") else     -- lui
                  b"01" when (opcode = b"000001") else     -- Memory
                  b"00";			                       -- ALU


end dataflow;
