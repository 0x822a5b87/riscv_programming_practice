#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

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
           "addi %[src], %[src], 2\n"                                   // src = src + 2
           "addi %[dst], %[dst], 2\n"                                   // dst = dst + 2
           "addi %[len], %[len], 2\n"                                   // len = len + 2
           "bltu %[len], %[size], 1b\n"                                 // if (len < size) goto 1b;
           : [dst] "+r" (dst), [len] "+r"(len), [tmp] "+r" (tmp)
           : [src] "r" (src), [size] "r" (size)
           : "memory"
       );
}

int main(void)
{
       int i;
       unsigned char *bufa = malloc(SIZE);
       if (!bufa)
             return 0;

     unsigned char *bufb = malloc(SIZE);
     if (!bufb) {
             free(bufa);
             return 0;
         }

     for (i = 0; i < SIZE; i++) {
             bufa[i] = i;
             printf("%d ", bufa[i]);
         }
       printf("\n");

       //printf("%p \n", bufa);
       swap_data(bufa, bufb, SIZE);
       //printf("%p \n", bufa);

       for (i = 0; i < SIZE; i++)
            printf("%d ", bufb[i]);
       printf("\n");

       free(bufa);
       free(bufb);

       return 0;
   }