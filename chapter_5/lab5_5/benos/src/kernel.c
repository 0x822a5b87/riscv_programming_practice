#include "uart.h"
#include "type.h"
#include "memset.h"

extern int macro_test_1(int l, int r);
extern int macro_test_2(int l, int r);

void asm_test()
{
	if (macro_test_1(100, 200) == 300) {
		uart_send_string("macro test 1 ok!\n");
	}

	if (macro_test_2(200, 500) == 700) {
		uart_send_string("macro test 2 ok!\n");
	}
}

void kernel_main(void)
{
	uart_init();
	uart_send_string("Welcome RISC-V!\r\n");

	asm_test();

	while (1) {
		;
	}
}
