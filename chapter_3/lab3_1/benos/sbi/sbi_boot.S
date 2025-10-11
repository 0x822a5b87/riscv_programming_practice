.section ".text.boot"

.globl _start
_start:
    # csrw Control and Status Register Write
    # mie  Machine Interrupt Enable
    # zero 零值寄存器
    # 代码将 mie 寄存器清零，全局禁用了机器模式下的所有中断。
    csrw mie, zero
    # Load Address 读取函数stacks_start地址到sp
    la sp, stacks_start
    
    # Load Immediate 读取立即数4096到t0
    li t0, 4096

    # 增加sp的值，需要注意的是：
    # 1. 我们修改了 sp 的值，增加了4096；
    # 2. stacks_start 中，通过 .skip 4096 跳转了相同的值；
    # 这也就是为什么我们最后程序可以正常执行的原因
    # 但是，这里有一个重要的概念，那就是：
    # stacks_start ，他既是这个函数（更准确来说是标签）的地址，同时也是栈指针的起点地址
    add sp, sp, t0

    # 这需要注意的是伪指令，实际可能被编译到如下
    #  ► 0x80000014 <_start+20>     addiw  a0, zero, 0x401     A0 => 0x401 (0x0 + 0x401)
    #    0x80000018 <_start+24>     slli   a0, a0, 0x15        A0 => 0x80200000 (0x401 << 0x15)
    li  a0, 0x80200000
    li  a1, 0x10

    # 这里需要注意， lw a1, a0 是一个错误的写法。因为编译器会混淆标签和寄存器
    # 所以，正确的写法是 lw a1, (a0)。表示使用a0地址中的数据进行寻址
    lw   a1, (a0)
    ld   a1, 0x10(a0)

    # lui 是 load upper immediate 的缩写，加载高20位，这里值得注意的是，当我们扩展立即数的时候是有符号扩展的
    # 1. 取指令中的 20 位立即数 imm。
    # 2. 将这个 20 位的 imm 符号扩展到 32 位（或 64 位）。
    # 3. 将扩展后的立即数左移 12 位。
    # 4. 将最终结果写入目标寄存器 rd。
    lui t0, 0x8034f # 0xffffffff8034f000
    lui t1, 0x400   # 0x400000

    tail sbi_main

.section .data
.align  12
.global stacks_start
stacks_start:
	.skip 4096
