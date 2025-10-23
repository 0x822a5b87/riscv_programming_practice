# 触发非法指令异常

## 通过修改指令数据触发

```c
void create_illegal_intr(void)
{
	int *p = (int *)trigger_load_access_fault;
	
	*p = 0xbadbeef;
}

create_illegal_intr();
// 此时，函数指针指向的地址是错误的指令了
trigger_load_access_fault();
```

## 在M模式访问M模式下的寄存器

以下的代码，都会触发非法指令异常，这是因为在 `ecall` 执行结束之后，回到他下一条指令的时候，已经从M模式转回了S模式。

```asm
.global access_csr_fault
access_csr_fault:
    ecall
    csrr a0, mstatus        // 触发非法指令异常
    ret
```

```asm
.global access_csr_fault
access_csr_fault:
    ecall
    mret                    // 触发非法指令异常
    ret
```