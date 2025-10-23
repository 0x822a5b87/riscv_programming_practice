#ifndef _ASM_RISCV_SBI_H
#define _ASM_RISCV_SBI_H

/*
 * SBI提供timer服务
 */
#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 0x1
#define SBI_CONSOLE_GETCHAR 0x2

// 声明一个 unsigned long 类型的变量 a0，该对象存储在寄存器 a0 中，对应的值是 arg0
// register unsigned long a0 asm ("a0") = (unsigned long)(arg0);

// 下面这个宏的含义是：
// 会将 SBI_CALL 的四个参数，存放到对应的 a0, a1, a2, a7 寄存器中
// ecall是riscv中为了从低特权模式进入高特权模式（例如U模式进入S模式）的一次主动的同步异常。
// ecall 指令本身不直接携带参数（指令编码中没有参数字段），但其输入参数和输出参数通过通用寄存器传递
// 
#define SBI_CALL(which, arg0, arg1, arg2) ({						\
	register unsigned long a0 asm ("a0") = (unsigned long)(arg0);	\
	register unsigned long a1 asm ("a1") = (unsigned long)(arg1);	\
	register unsigned long a2 asm ("a2") = (unsigned long)(arg2);	\
	register unsigned long a7 asm ("a7") = (unsigned long)(which);	\
	asm volatile ("ecall"											\
		      : "+r" (a0)											\
		      : "r" (a1), "r" (a2), "r" (a7)						\
		      : "memory");											\
	a0;																\
})

/* 
 * 陷入到M模式，调用M模式提供的服务。
 * SBI运行到M模式下
 */
#define SBI_CALL_0(which) SBI_CALL(which, 0, 0, 0)
#define SBI_CALL_1(which, arg0) SBI_CALL(which, arg0, 0, 0)
#define SBI_CALL_2(which, arg0, arg1) SBI_CALL(which, arg0, arg1, 0)

static inline void sbi_set_timer(unsigned long stime_value)
{
	SBI_CALL_1(SBI_SET_TIMER, stime_value);
}

static inline void sbi_putchar(char c)
{
	SBI_CALL_1(SBI_CONSOLE_PUTCHAR, c);
}

static inline void sbi_put_string(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		sbi_putchar((char) str[i]);
}
#endif /*_ASM_RISCV_SBI_H*/
