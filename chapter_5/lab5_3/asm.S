.section .text
.align 2
.global main
main:
    addi sp, sp, 32
    sd   ra, 24(sp)

    li a0, 100
    li a1, 200
    call cmp

    ld   ra, 24(sp)
    addi sp, sp, -32
    ret