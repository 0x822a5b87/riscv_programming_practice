static inline void __raw_writel(u32 val, volatile void __iomem *addr)
{
    // *addr = val
    asm volatile("sw %0, 0(%1)" : : "r" (val), "r" (addr));
}