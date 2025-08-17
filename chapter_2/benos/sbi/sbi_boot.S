/*
 * 1. 声明这段代码属于 .text.boot 段（自定义的启动代码段）。
 * 2. 结合之前的链接脚本可知，.text.boot 段会被链接到内存的 0x80000000 地址（RISC-V 处理器的默认启动地址），确保处理器上电后首先执行这段代码。
 */
.section ".text.boot"

/*
 * .globl _start 的主要作用是声明 _start 为一个全局符号（global symbol），作用是初始化一个 4KB 的栈
 */
.globl _start
_start:
	la sp, stacks_start             ; load address  将栈底地址（stacks_start）加载到栈指针寄存器 sp
	li t0, 4096                     ; load imediate 加载立即数 4096 到临时寄存器 t0（栈大小为 4KB）
	add sp, sp, t0                  ; 栈指针 sp = 栈底 + 4096 → 指向栈顶（栈向低地址生长）

	/* goto C */
	tail sbi_main                   ; 无条件跳转到 sbi_main 函数（不保存返回地址，因为不会返回）

.section .data                      ; 声明属于数据段（.data）
.align  12                          ; .align 指定按照 2^12 进行对齐
.global stacks_start                ; 声明 stacks_start 为全局符号，供汇编代码引用
stacks_start:
	.skip 4096
