#include <stdio.h>
#include <string.h>
#include <stdlib.h>
　
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
　
#define SIZE 8*100
　
int main()
{
  int i;
　
  char *src = malloc(SIZE);
  char *dst = malloc(SIZE);
　
  if (!src || !dst)
      return 0;
　
  printf("0x%lx 0x%lx\n", (unsigned long)src, (unsigned long)dst);
　
  for (i = 0; i < SIZE; i+=8)
      *(unsigned long *)(src + i) = 0x55;
　
  my_memcpy_asm_test((unsigned long)src, (unsigned long)dst, SIZE);
　
  for (i = 0; i < SIZE; i+=8) {
      if (*(unsigned long *)(dst + i) != 0x55) {
          printf("data error %lx, i %d\n", (unsigned long)(dst + i), i);
          goto free;
      }
  }
　
  printf("test done\n");
　
free:
  free(src);
  free(dst);
  return 0;
}