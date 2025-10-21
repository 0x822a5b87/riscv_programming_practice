# readme

## atomic.h 

```c
void my_atomic_add(unsigned long val, void *p);
```

## main.c

```c
#include "stdio.h"
#include "atomic.h"

int main(int argc, char const *argv[])
{
    int a = 10;
    printf("a = %d\n", a);
    my_atomic_add(20, &a);
    printf("a = %d\n", a);
    return 0;
}
```

## atomic.c

```c
static inline void my_atomic_add(unsigned long val, void *p)
{
    unsigned long tmp;
    long result;
    asm volatile (
            "1: lr.d %0, (%2)\n"
            "add %0, %0, %3\n"
            "sc.d.rl %1, %0, (%2)\n"
            "bnez %1, 1b\n"
            "fence    rw, rw\n"
            : "+r" (tmp), "+r"(result)
            : "r"(p), "r" (val)
            : "memory"
             );
}
```

>为什么`gcc atomc.h atomic.c main.c` 提示 main.c:(.text+0x32): undefined reference to `my_atomic_add'

由于inline本身的目的就是把代码通过字符的方式嵌入到调用中，避免函数调用引入的开销。

所以gcc在实现的时候，不会在目标文件中生成一个对应的函数，所以会在链接期间报错。

所以，有两种解决方法：

1. 不使用 inline，这样生成的目标文件中就会有对应的函数可以在链接期间被链接；
2. 在atomic.h中声明inline函数，并且在main函数中inlucde引用，在预处理期间让两个文件合并。

