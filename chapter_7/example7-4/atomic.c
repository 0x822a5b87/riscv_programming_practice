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

