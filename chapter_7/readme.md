# 内嵌汇编代码

## 思考题

>在内嵌汇编代码中，修饰词“volatile”​“inline”​“goto”的作用分别是什么？

### volatile

告诉编译器：该变量的值可能被程序之外的因素（如硬件、中断、多线程）修改，因此编译器不能对其进行优化（如缓存到寄存器、省略读取）。

例如，线程 A 循环读取 int count = 0，线程 B 负责递增 count。如果 count 不加 volatile，编译器可能会优化线程 A 的读取（假设 count 不变），导致线程 A 始终看到 count = 0，即使线程 B 已经修改了它。
注意：volatile 不保证线程安全（无法解决竞态条件），但能确保线程 A 每次都从内存读取 count 的最新值（而非寄存器缓存）。

### inline

inline 也是 C/C++ 关键字，用于建议编译器：将函数调用替换为函数体本身（内联展开），而不是生成跳转指令。

在汇编层面的体现是：普通函数调用会生成 call 跳转指令（跳转到函数地址执行，结束后 ret 返回）。
inline 函数会被编译器 “嵌入” 到调用处，汇编代码中直接包含函数体的指令（无跳转），减少函数调用的开销（如栈操作、跳转延迟）。

核心作用：通过消除函数调用的汇编指令（call/ret 及栈操作），减小代码体积（对短函数）并提高执行效率，尤其适合频繁调用的小函数。

>在内嵌汇编代码的输出部分里，​“=”和“+”分别代表什么意思？

在 GCC 内嵌汇编（Extended Asm）中，存在只读约束，用字母 r、m、i 等表示，而 `=` 和 `+` 是专门用于 `输出操作数` 的约束前缀，用于区分操作数的读写权限。具体规则如下：

内嵌汇编的操作数分为输入（input） 和输出（output） 两类：

- 输入操作数：仅被汇编代码读取，默认是 “只读” 的，无需前缀，直接用约束字母表示（如 r、m）。
- 输出操作数：会被汇编代码修改，需要用前缀声明权限：
  - `=`：表示 “只写”（输出操作数，汇编代码只会修改它，不会读取其初始值）。
  - `+`：表示 “读写”（输出操作数，但汇编代码会先读取其初始值，再修改）。

>- output（输出操作数）：汇编代码会修改的变量（即汇编执行后，该变量的值会被更新）。
>input（输入操作数）：汇编代码仅读取的变量（即汇编执行过程中，该变量的值不会被修改，仅作为输入数据）。

```asm
asm 修饰词(
           指令部分
           : 输出部分
           : 输入部分
           : 损坏部分)
```

在 GCC 内嵌汇编中，“损坏部分”（Clobber List）是一个可选的参数列表，用于告诉编译器：哪些寄存器或资源在汇编指令执行过程中被 “意外修改” 了，需要编译器在生成代码时进行特殊处理（如提前保存、避免使用），以防止汇编指令破坏程序的执行环境。

常见的损坏像

>寄存器

```asm
asm volatile (
    "mov $1, %%ecx"  // 直接修改 %ecx
    : 
    : 
    : "%ecx"  // 声明 %ecx 被修改（损坏）
);
```

>特殊资源，如内存。"memory" 修饰符的核心作用是强制编译器放弃对内存操作的优化，确保汇编指令对内存的修改能被后续代码 “看见”，同时后续代码对内存的访问也不会被优化掉。

当在内嵌汇编中声明 "memory" 时，编译器会做以下处理：
1. 禁止寄存器缓存内存变量：编译器通常会将频繁访问的内存变量缓存到寄存器中（减少内存读写开销）。但 str %1, [%0] 直接修改了内存地址 %0 处的值，若后续 C 代码访问该地址的变量，"memory" 会强制编译器重新从内存中读取该变量（而非使用寄存器中的旧值），确保读到的是汇编修改后的新值。
2. 阻止内存操作重排序：编译器可能会优化代码中内存操作的顺序（只要逻辑等价）。"memory" 会告诉编译器：汇编指令对内存的修改与前后的 C 代码内存操作存在依赖关系，不能随意重排序（例如，汇编写内存的操作不能被移到后续 C 代码读内存之后）。
3. 不特指 addr 和 value：addr 和 value 是输入操作数（通过寄存器传递的值），"memory" 不直接要求 “重新加载 addr 和 value”，而是针对所有内存地址（尤其是 addr 指向的目标内存）。addr 和 value 本身如果是普通变量，其值是否被重新加载取决于它们是否被后续代码使用及编译器优化，"memory" 不直接干预寄存器中 addr 和 value 的值，只保证内存访问的准确性。

```asm
asm volatile (
    "str %1, [%0]"  // 直接写内存地址 %0
    : 
    : "r"(addr), "r"(value) 
    : "memory"  // 声明内存被修改
);
```

```asm
int x = 0;
int *addr = &x;
int value = 100;

// 内嵌汇编修改 x 的值为 100
asm volatile (
    "str %1, [%0]"
    : 
    : "r"(addr), "r"(value) 
    : "memory"  // 声明内存被修改
);

// 后续代码访问 x
printf("%d\n", x);  // 若无 "memory"，编译器可能优化为直接用 x 的初始值 0（缓存到寄存器）
                    // 有 "memory" 时，会强制从内存读取 x，得到 100
```


```c
int a = 10, b = 20, c;
// 内嵌汇编：c = a + b（a和b是只读输入，c是只写输出）
asm volatile (
    "add %0, %1, %2"  // 汇编指令：%0 = %1 + %2
    : "=r" (c)        // 输出：c 只写（=）
    : "r" (a), "r" (b) // 输入：a和b 只读（无前缀，默认只读）
);
```


>在内嵌汇编代码中，如何表示输出部分和输入部分的参数？

>内嵌汇编代码与C语言宏结合时，​“#”与“##”分别代表什么意思？

### 例子7-4

```c
void my_atomic_add(unsigned long val, void *p)
{
    unsigned long tmp;
    long result;
    asm volatile (
            // 1: 是汇编语言中的局部标号；
            // lr.d 是 RISC-V 架构中的一条指令，属于原子操作指令集（Atomic Instructions）的一部分，全称为 Load Reserved Doubleword（加载保留双字）。
            // lr.d 是 RISC-V 中用于实现原子内存操作的核心指令之一，与 sc.d（Store Conditional Doubleword，条件存储双字）配合使用，
            // 构成 “加载 - 修改 - 存储”（Load-Modify-Store）的原子操作序列。
            // 从指定内存地址读取 64 位数据到寄存器，同时在 CPU 中对该内存地址设置 “保留”（Reserved）标记，用于监控该地址是否被其他硬件线程（或处理器）修改，
            // 为后续 sc.d 的条件存储提供依据。
            "1: lr.d %0, (%2)\n"                    // tmp = *p
            "add %0, %0, %3\n"                      // tmp = tmp +val

            // sc.d 是 “条件存储”（Store Conditional）指令，尝试将 tmp 的新值写回 p 指向的内存。
            // 若 lr.d 标记的 “保留” 状态未被破坏（即该内存地址未被其他处理器修改），则写入成功，result（%1）被设为 0。
            // 若 “保留” 状态被破坏（地址被修改），则写入失败，result 被设为非 0。
            "sc.d.rl %1, %0, (%2)\n"                // if reserved { *p = tmp; result = zero; } else { reslut = not zero; }
            "bnez %1, 1b\n"                         // if (result != 0) goto 1b; 1b表示 1back跳转到上面的1
            // fence 是 RISC-V 的内存屏障指令，用于控制内存操作的顺序，防止编译器或 CPU 对内存访问进行重排序。rw, rw 是屏障的参数，含义是：
            // 第一个 rw：表示当前指令之前的所有读（r）和写（w）操作必须完成后，才能执行屏障之后的指令。
            // 第二个 rw：表示屏障之后的所有读（r）和写（w）操作必须等待屏障完成后才能开始。
            "fence    rw, rw\n"
            : "+r" (tmp), "+r"(result)              // 出参
            : "r"(p), "r" (val)                     // 入参
            : "memory"                              // 提示编译器内存被修改了
             );
}
```


```c
1    static inline void my_atomic_add(unsigned long val, void *p)
2    {
3        unsigned long tmp;
4        int result;
5        asm volatile (
                 // 注意，这里和之前的例子参数是不完全一样的
                 // 1: lr.d %0, (%2)\n —— 用寄存器里的 “地址” 访问内存（对应 void* p）
                 // 寄存器 %2 里存储的是 p 的值（比如 0x1000，也就是共享变量的内存地址）。
                 // 1: lr.d %0, %2\n   —— 直接用寄存器里的 “值”（对应 *(unsigned long *) p 的结果）
                 // 寄存器 %2 里存储的是 *(unsigned long *)p 的结果（比如 0x1234，也就是内存地址 0x1000 里存的具体数值）。
                 // 也就是说：
                 // 假设存在 p = &val
                 // 1: lr.d %0, (%2)\n 这里实际相当于 %0 = *p
                 // 1: lr.d %0, %2\n    这里相当于 %0 = %2
                 // 最终的结果是：第一条指令去读取了内存中的值，而第二条指令中，则直接读取了%2对应的寄存器的值。
6                "1: lr.d %0, %2\n"                                         // tmp = *p
7                "add %0, %0, %3\n"                                         // tmp = tmp + val
8                "sc.d.rl %1, %0, %2\n"                                     // if (reserved) { *p = tmp; result = zero; } else { result = not zero;}
9                "bnez %1, 1b\n"                                            // if (result != 0) { goto 1b; }
10               : "+r" (tmp), "+r"(result), "+A"(*(unsigned long *)p)      // 在这里，我们通过 'A' 指定了将 *p 的值存到了通用寄存器
11               : "r" (val)
12               : "memory"
13                );
14   }
```

### example 7-5

```asm
int add(int i, int j)
{
    int res = 0;

    asm volatile (
    "add %[result], %[input_i], %[input_j]"         // res = i + j
    : [result] "=r" (res)
    : [input_i] "r" (i), [input_j] "r" (j)
    );

    return res;
}
```

### 7.1.5

```c
#define ATOMIC_OP(op, asm_op, I, asm_type, c_type, prefix)		\
static __always_inline							\
void arch_atomic##prefix##_##op(c_type i, atomic##prefix##_t *v)	\
{									\
	__asm__ __volatile__ (						\
		"	amo" #asm_op "." #asm_type " zero, %1, %0"	\
		: "+A" (v->counter)					\
		: "r" (I)						\
		: "memory");						\
}

#define ATOMIC_OPS(op, asm_op, I)					\
        ATOMIC_OP (op, asm_op, I, w, int,   )				\
        ATOMIC_OP (op, asm_op, I, d, s64, 64)
```

### 7.1.7 

```c
static void my_memcpy_asm_test(unsigned long src, unsigned long dst, unsigned long size)
{
  unsigned long tmp = 0;
  unsigned long end = src + size;
　// 这段代码其实存在一个BUG，如果size等于0，那么本身不应该执行复制操作，但实际会执行一次复制操作。
  asm volatile (
          "1: ld %1, (%2)\n"                        // tmp = *src
          "sd %1, (%0)\n"                           // *dst = tmp
          "addi %0, %0, 8\n"                        // dst += 1
          "addi %2, %2, 8\n"                        // src += 1
          "blt %2, %3, 1b"                          // if (src > end) goto 1b;
          : "+r" (dst), "+r" (tmp), "+r" (src)
          : "r" (end)
          : "memory");
}
```

## 7.2 案例分析

```c
static void swap_data(unsigned char *src, unsigned char *dst,unsigned int size)
{
        unsigned int len = 0;
        unsigned int tmp;

       asm volatile (
           "1: lhu a5, (%[src])\n"                                      // a5 = *src，src 是 unsigned char*，他指向的数据只有一个字节，但是这里读取两个字节的数据会导致内存越界
           "sll a6, a5, 8\n"                                            // a6 = a5 << 8
           "srl a7, a5, 8\n"                                            // a7 = a5 >> 8
           "or %[tmp], a6, a7\n"                                        // tmp = a6 | a7
           "sh %[tmp], (%[dst])\n"                                      // *dst = tmp，dst 是 unsigned char*，这里写入两个字节，同样会造成内存越界
           "addi %[src], %[src], 2\n"                                   // src = src + 2，这里修改了 src，所以src必须可写
           "addi %[dst], %[dst], 2\n"                                   // dst = dst + 2
           "addi %[len], %[len], 2\n"                                   // len = len + 2
           "bltu %[len], %[size], 1b\n"                                 // if (len < size) goto 1b;
           : [dst] "+r" (dst), [len] "+r"(len), [tmp] "+r" (tmp),
             [src] "+r" (src)                                           // src 应该作为输出参数，并且可读可写
           : [size] "r" (size)
           : "memory", "a5", "a6", "a7"                                 // a5, a6, a7 三个寄存器被修改了，需要提醒编译器
       );
}
```