# readme

## asm

```asm
	.global asm_bl_test
asm_bl_test:
	mv x18, ra 			# 把ra寄存器的值保存到临时寄存器中
	call sel_test		# 调用函数
	nop					# 保证兼容性
	mv ra, x18			# 恢复父函数的返回地址
	ret
```