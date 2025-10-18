    .section .text
    .align 2
    .global  cmp
cmp:
    addi     sp, sp, -32; # sp = sp + -32
    sd       ra, 24(sp)   #

    bgt      a0, a1, .L1
    mv       a0, a1
.L1:
    ld       ra, 24(sp)
    addi     sp, sp, 32
    ret