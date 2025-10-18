#include "uart.h"
#include "type.h"
#include "memset.h"

extern void load_store_test(void);
extern void shift_test(void);
extern void add_sub_test(void);
extern void branch_test(void);
extern void my_memcpy_test(void);
extern unsigned long compare_and_return(unsigned long a, unsigned long b);
extern unsigned long sel_test(unsigned long a, unsigned long b); 
extern void bl_test();

extern long func_addr[];
extern char* func_name[];
extern unsigned int func_len;

void print_func_name(const int addr)
{
	for (int i = 0; i < func_len; i++) {
		if (func_addr[i] == addr) {
			uart_send_string(func_name[i]);
			uart_send_string("\n");
		}
	}
}


void asm_test(void)
{
	unsigned long val1, val2;

	load_store_test();
	shift_test();
	add_sub_test();
	branch_test();

	my_memcpy_test();

	/*lab3-4: memset函数实现*/
	memset((void *)0x80210005, 0x55, 40);

	/*lab3-5:  条件跳转指令1*/
	val1 = compare_and_return(10, 9);
	if (val1 == 0)
		uart_send_string("lab3-5: compare_and_return ok\n");

	val2 = compare_and_return(9, 10);
	if (val2 == 0xffffffffffffffff)
		uart_send_string("lab3-5: compare_and_return ok\n");

	val1 = sel_test(0, 9);
	val2 = sel_test(5, 2);

	bl_test();
	uart_send_string("lab3-7: branch test ok\n");
}

void kernel_main(void)
{
	uart_init();
	uart_send_string("Welcome RISC-V!\r\n");

	asm_test();

	print_func_name(0x800880);
	print_func_name(0x800860);
	print_func_name(0x800800);

	while (1) {
		;
	}
}
