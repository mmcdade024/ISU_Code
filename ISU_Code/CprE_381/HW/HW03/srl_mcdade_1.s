    .text
    .globl main
main:
    li $t0, 0b0000111100001111  # Load test value
    srl $t1, $t0, 4             # Shift right by 4
    # Expected: $t1 = 0x00000F00 (111100000000)