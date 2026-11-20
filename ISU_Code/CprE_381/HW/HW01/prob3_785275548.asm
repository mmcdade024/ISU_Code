.data
str1: .asciiz "Please enter the index of the first value:\n"
str2: .asciiz "Please enter the index of the second value:\n"
str3: .asciiz "Please enter the index to store the result:\n"
.align 2
vals: .word 25 1 4 10 381 42 100 60 0 12 25
result: .word 0

.text
.globl main

main:
   
    addi $s1, $zero, 0  

    li $v0, 4
    la $a0, str1
    syscall

    li $v0, 5
    syscall
    move $t0, $v0  

    li $v0, 4
    la $a0, str2
    syscall
    
    li $v0, 5
    syscall
    move $t1, $v0

    li $v0, 4
    la $a0, str3
    syscall
    
    li $v0, 5
    syscall
    move $t2, $v0

    la $t3, vals
    sll $t0, $t0, 2  
    addu $t3, $t3, $t0
    lw $s0, 0($t3)

    la $t3, vals
    sll $t1, $t1, 2
    addu $t3, $t3, $t1
    lw $s1, 0($t3)

    sub $t4, $s0, $s1
    abs $t4, $t4      
    add $t5, $s0, $s1  
    sub $t6, $t5, $t4 
    srl $t6, $t6, 1   

    la $t3, vals
    sll $t2, $t2, 2 
    addu $t3, $t3, $t2
    sw $t6, 0($t3) 

    li $v0, 1
    move $a0, $t6
    syscall

    li $v0, 10
    syscall