    .align  3
    .global asm_add_c
asm_add_c:
    addi    sp, sp, -16;  # 虽然我们只存sp，但是栈帧最小需要16字节
    sd      ra, 8(sp)

    addw    a0, a0, a1;   # a0 = a0 + a1

    mv      a0, a0        # 函数调用三部曲，设置返回值，还原return address，还原sp，这里其实可以省略这一条指令
    ld      ra, 8(sp);
    addi    sp, sp, 16;

    ret

    .global asm_func1
asm_func1:
    addi    sp, sp, -16;
    sd      ra, 8(sp)

    li      a0, 1;
    li      a1, 2;
    call    asm_add_c

    mv      a0, a0        # 函数调用三部曲，设置返回值，还原return address，还原sp，这一步同样可以省略
    ld      ra, 8(sp);
    addi    sp, sp, 16;

    ret

    .global asm_add_c_jal
asm_add_c_jal:
    addi    sp, sp, -16;   # sp = sp + 16
    sd      ra, 8(sp)
    addw    a0, a0, a1
    mv      a0, a0        # 函数调用三部曲，设置返回值，还原return address，还原sp，这一步同样可以省略
    ld      ra, 8(sp);
    addi    sp, sp, 16;
    ret

    .global asm_func1_jal
asm_func1_jal:
    addi    sp, sp, -16;   # sp = sp + 16
    sd      ra, 8(sp)

    li      a0, 1;
    li      a1, 2;
    jal     asm_add_c_jal

    mv      a0, a0        # 函数调用三部曲，设置返回值，还原return address，还原sp，这一步同样可以省略
    ld      ra, 8(sp);
    addi    sp, sp, 16;

    ret