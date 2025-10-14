# readme

## source code

```c++
unsigned long sel_test(unsigned long a, unsigned long b) 
{
   if (a == 0)
           return b+2;
   else
           return b-1;
}
```

## asm

```asm
	.global sel_test
sel_test:
	beqz    a0, .L5
.L4:
	addi    a0, a1, -1;                # a0 = a1 + -1
	ret
.L5:
	addi    a0, a1, 2;                 # a0 = a1 + 2
	ret
```