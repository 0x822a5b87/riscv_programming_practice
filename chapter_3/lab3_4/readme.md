# 实验3-4: memset()函数的实现

## 实验1

>memset()函数的C语言原型如下。假设内存地址s按16字节对齐，count也按16字节对齐。请使用RISC-V汇编指令实现这个函数。

```c++
void *memset(void *s, int c, size_t count)
{
  char *xs = s;
　
  while (count--)
      *xs++ = c;
  return s;
}
```

## 实验2

>假设内存地址s以及count不按16字节对齐，如memset(0x80210004, 0x55, 102)，请继续优化memset()函数。