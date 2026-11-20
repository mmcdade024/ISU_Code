.data
something: .word 0
my_array: .word 2 3 5 8 

.text

ori $t1, $zero, 0x1001
sll $t1, $t1, 16
lb $t0, 0($t1)
ori $t0, $t0, 0xF0
sb $t0, 0($t1)

# 0x1001 into upper half of $t1

# Increment address in $t3 by 3 ints

# Put the address of array label in $s3

# Initialize $t5 to an index represented by an immediate; increment that element of the array

# Generate an unaligned access – what happens in MARS?

# Write code snippet to test word alignment (0x0…0 for aligned, else for unaligned)

# Write a code snippet to determine/verify the endianness of your MIPS machine
