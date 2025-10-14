# readme

## source code

```c
extern int asm_add_c(int a, int b);
extern int asm_func1(void);

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

void kernel_main(void)
{
	uart_init();
	int v1 = func1();
	int v2 = asm_add_c(1,2);
	if (v1 == v2) {
		uart_send_string("asm add ok\r\n");
	} else {
		uart_send_string("asm add failed\r\n");
	}
}
```

## asm

```asm
    .align  3
    .global asm_add_c
asm_add_c:
    addi    sp, sp, -16; # 虽然我们只存sp，但是栈帧最小需要16字节
    sd      ra, 8(sp)

    addw    a0, a0, a1;  # a0 = a0 + a1

    mv      a0, a0       # 函数调用三部曲，设置返回值，还原return address，还原sp，这里其实可以省略这一条指令
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

    mv      a0, a0       # 函数调用三部曲，设置返回值，还原return address，还原sp，这一步同样可以省略
    ld      ra, 8(sp);
    addi    sp, sp, 16;
```