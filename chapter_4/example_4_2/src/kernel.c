#include "uart.h"

extern int asm_add_c(int a, int b);
extern int asm_func1(void);

extern int asm_add_c_jal(int a, int b);
extern int asm_func1_jal(void);

int add_c(int a, int b)
{
	return a + b;
}

int func1(void)
{
	int a = 1;
	int b = 2;

	return add_c(a, b);
}

int reset_ra(void)
{
	int a = 0x10;
	int b = 0x1;

	return a + b;
}

void kernel_main(void)
{
	uart_init();
	int v1 = func1();
	reset_ra();
	int v2 = asm_func1();
	reset_ra();
	int v3 = asm_func1_jal();
	if (v1 == v2) {
		uart_send_string("asm add ok\r\n");
	} else {
		uart_send_string("asm add failed\r\n");
	}

	if (v1 == v3) {
		uart_send_string("asm add jal ok\r\n");
	} else {
		uart_send_string("asm add jal failed\r\n");
	}
}
