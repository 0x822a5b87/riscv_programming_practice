#include "uart.h"
#include "type.h"
#include "memset.h"


extern void load_store_test(void);
extern void pc_related_test(void);
extern void shift_test(void);
extern void add_sub_test(void);
extern unsigned long compare_and_return(unsigned long a, unsigned long b);
extern unsigned long sel_test(unsigned long a, unsigned long b);
extern void my_memcpy_test(void);

bool cmp_int(const int *p1, const int *p2) {
	int v1 = *p1;
	int v2 = *p2;
	return v1 == v2;
}

void memcpy_test() {
	int *p1 = (int *) 0x80200000;
	int *p2 = (int *) 0x80210000;
	if (cmp_int(p1, p2)) {
		uart_send_string("memcpy is ok already!");
	} else {
		my_memcpy_test();
		if (cmp_int(p1, p2)) {
			uart_send_string("memcpy ok!\n");
		} else {
			uart_send_string("memcpy failed!\n");
		}
	}
}

void memset_test(void) {
	// 这里一定需要注意的是，由于我们在 __my_memset_16bytes_asm 中，我们是按照8个字节进行操作的
	// 也就是说，我们的指针必须也是8个字节的，否则会导致结果异常
	double *p1 = (double *) 0x80210005;
	my_memset(p1, 0x55, 0);
	if (*p1 == 0x55) {
		uart_send_string("memcpy for case 1 error!\n");
	}

	my_memset(p1, 0x55, 1);
	if (*p1 != 0x55) {
		uart_send_string("memcpy for case 2 error!\n");
	}

	my_memset(p1, 0x55, 10);
	for (int i = 0; i < 2; i++) {
		if (*(p1 + i) != 0x55) {
			uart_send_string("memcpy for case 3 error!\n");
		}
	}
}

void asm_test(void)
{
	unsigned long val1, val2;

	load_store_test();
	pc_related_test();
	shift_test();
	add_sub_test();

	val1 = compare_and_return(10, 9);
	if (val1 == 0)
		uart_send_string("compare_and_return ok\n");
	else
		uart_send_string("compare_and_return fail\n");

	val2 = compare_and_return(9, 10);
	if (val2 == 0xffffffffffffffff)
		uart_send_string("compare_and_return ok\n");
	else
		uart_send_string("compare_and_return fail\n");

	// sel_test 函数逻辑是：
	// 如果第一个参数为0，则把第二个参数+9并返回。否则把第二个参数-1返回
	val1 = sel_test(0, 9);
	if (val1 == 11)
		uart_send_string("sel test ok\n");

	val2 = sel_test(5, 2);
	if (val2 == 1)
		uart_send_string("sel test ok\n");

	memcpy_test();
	memset_test();
}

void kernel_main(void)
{
	uart_init();
	uart_send_string("Welcome RISC-V!\r\n");

	asm_test();

	while (1)
	{
		;
	}
}
