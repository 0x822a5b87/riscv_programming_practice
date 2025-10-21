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
10               : "+r" (tmp), "+r"(result), "+A"(*(unsigned long *)p)      // 
11               : "r" (val)
12               : "memory"
13                );
14   }