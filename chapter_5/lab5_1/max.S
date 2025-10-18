    # as max.S -o max.o && ld max.o -o max -Map max.map -lc --dynamic-linker /lib/ld-linux-riscv64-lp64d.so.1 && ./max
    .section .rodata
format:
    .string  "%d"

    .section .data
    .align   3
start:
    .string  "start program.\n"
print_data_1:
    .string  "we have %d numbers\n"
print_data_2:
.string "the max number is :
    %d,      remaining: %d\n"
end:
    .string  "end program.\n"


    .section .text                  # 切换到代码段
    .align   2                      # 2^2字节对齐

max:
    addi     sp, sp, -32            # 初始化栈，保存调用信息
    sd       ra, 24(sp)

    la       a0, format             # a0 = format
    addi     a1, sp, 0              # a1 == &sp
    call     scanf

    lw       a1, 0(sp)              # a1 = *sp = len(arr)
    la       a0, print_data_1
    call     printf

    lw       a2, 0(sp)              # 0(sp) = len(arr)

    li       a0, 0
    sw       a0, 16(sp)             # 16(sp) = 0

.L1:
    lw       a0, 0(sp)
    beqz     a0, .L3

    la       a0, format
    addi     a1, sp, 8              # 8(sp) = new_number
    call     scanf

    lw       a0, 8(sp)
    lw       a1, 16(sp)
    bgt      a1, a0, .L4
    sw       a0, 16(sp)

.L4:
    la       a0, print_data_2
    lw       a1, 16(sp)
    lw       a2, 0(sp)
    call     printf

    lw       a0, 0(sp)
    addi     a0, a0, -1
    sw       a0, 0(sp)              # len(arr) = len(arr) - 1

    j        .L1
.L3:
    la       a0, end
    call     printf

    li       a0, 0                  # 设置返回值
    ld       ra, 24(sp)             # 恢复栈和调用信息
    addi     sp, sp, 32
    ret