# lab

## lab5_4

使用riscv实现表，并在C语言中通过查找表获取函数名称。例如：

```bash
0x800800 -> func_a
0x800860 -> func_b
0x800880 -> func_c
```

实现函数 `print_func_name(addr)`，输出地址 `0x800800`，输出 `func_a`

## 关于riscv中的内存布局

假设我们目前的代码实现逻辑是这样的：

汇编代码：

```assembly
.align 3
.global func_name
func_name:
	.string "func_a"
	.string "func_b"
	.string "func_c"
```

C代码

```c
extern char* func_name[];
```

我们会发现，读到的数据非常的奇怪，这是因为在汇编中的声明的 `func_name` 内存布局，和我们实际的内存布局不一样。

>汇编中的内存布局：在汇编中，我们的 `func_name` 实际上是一连串的 `char`，而非一个指针。

```
地址: 内容
func_name → 'f','u','n','c','_','a','\0','f','u','n','c','_','b','\0',...
```

所以当我们在DEBUG的时候，编译器会将 `f` 当做一个指针，并去访问这个指针指向的地址。而这个访问得到的结果是完全随机的。

>C语言中的内存布局：`func_name` 指向的是一个 `char*` 数组，它认为接下来的内存数据是一连串的指针。

```
地址: 内容
func_name → 指针1（指向"func_a"）, 指针2（指向"func_b"）, 指针3（指向"func_c"）
```

这个导致了最终结果的不一样，我们有两个方法来解决这个问题：

### 方案1

方案1中，我们将 func_name 当做一个 `char *`，我们手动的来解析这些字符串。

#### RISCV

```asm
.align 3
.global func_addr
func_addr:
	.quad 0x800800
	.quad 0x800860
	.quad 0x800880

.align 3
.global func_name
func_name:
	.string "func_a"
	.string "func_b"
	.string "func_c"

.align 3
.global func_len
func_len:
	.INT 3
```

#### C

```c
extern long func_addr[];
extern char func_name;
extern unsigned int func_len;

void print_func_name(int addr)
{
	const char *start = &func_name;
	for (int i = 0; i < func_len; ++i)
	{
		char name[1024];
		for (unsigned int j = 0; *start != '\0'; ++j) {
			name[j] = *start++;
		}
		start++;

		if (func_addr[i] == addr) {
			uart_send_string(name);
			uart_send_string("\n");
		}
	}
}
```

### 方案2

>方案2更简单优雅，我们通过声明 `func_name` 为 `char *` 数组来实现。

#### 汇编代码

```asm
.align 3
.global func_addr
func_addr:
	.quad 0x800800
	.quad 0x800860
	.quad 0x800880

.func_a_name: .string "func_a"
.func_b_name: .string "func_b"
.func_c_name: .string "func_c"

.align 3
.global func_name
func_name:
    .quad .func_a_name
    .quad .func_b_name
    .quad .func_c_name

.align 3
.global func_len
func_len:
	.INT 3
```

#### C

```c
void print_func_name(const int addr)
{
	for (int i = 0; i < func_len; i++) {
		if (func_addr[i] == addr) {
			uart_send_string(func_name[i]);
			uart_send_string("\n");
		}
	}
}
```