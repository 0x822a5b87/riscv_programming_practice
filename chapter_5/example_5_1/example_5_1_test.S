# 以“.”字符开始的指令是汇编器能识别的伪操作，它不会直接被翻译成机器指令，而由汇编器来进行预处理
	.section .data          # 指定.data段
	.align   3              # .align是对齐伪操作，参数为3，因此对齐的字节大小为23，即接下来的数据所在的起始地址能被8整除。
my_data1: # 分配一个初始值为100，大小为四个字节的内存块，并且绑定到标签 my_data
	.word    100
my_data2: # 分配一个初始值为50，大小为四个字节的内存块，并且绑定到标签 my_data
	.word    50
print_data: # 分配字符串
.string "data:
	%d\n"

	.section .text          # 切换到代码段
	.align   2              # 2^2字节对齐
	.global  main
main:
	addi     sp, sp, -16    # 初始化栈，保存调用信息
	sd       ra, 8(sp)

	lw       t0, my_data1   # 100 + 50
	lw       t1, my_data2
	add      a1, t0, t1

	la       a0, print_data # 调用printf
	call     printf

	li       a0, 0          # 设置返回值

	ld       ra, 8(sp)      # 恢复栈和调用信息
	addi     sp, sp, 16
	ret


