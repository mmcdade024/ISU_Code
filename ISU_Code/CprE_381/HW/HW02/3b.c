    .data
str1:   .asciiz "hello"
char1:  .byte 'e'
str2:   .asciiz "world"
char2:  .byte 'z'
str3:   .asciiz "assembly"
char3:  .byte 'm'
null_msg: .asciiz "NULL"
result_msg: .asciiz "Result: "

    .text
    .globl main
main:
    la $a0, str1
    lb $a1, char1
    jal strchr
    jal print_result

    la $a0, str2
    lb $a1, char2
    jal strchr
    jal print_result

    la $a0, str3
    lb $a1, char3
    jal strchr
    jal print_result

    li $v0, 10
    syscall

print_result:
    move $t0, $v0
    la $a0, result_msg
    li $v0, 4
    syscall

    beqz $t0, print_null
    move $a0, $t0
    li $v0, 4
    syscall
    j print_end

print_null:
    la $a0, null_msg
    li $v0, 4
    syscall

print_end:
    li $a0, 10
    li $v0, 11
    syscall
    jr $ra

strchr:
    lb $t0, 0($a0)
    beq $t0, $a1, found
    beqz $t0, not_found
    addi $a0, $a0, 1
    j strchr

found:
    move $v0, $a0
    jr $ra

not_found:
    li $v0, 0
    jr $ra