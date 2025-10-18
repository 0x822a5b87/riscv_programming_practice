#include "stdio.h"

int cmp(int l, int r)
{
    int max = l;
    if (r > l)
    {
        max = r;
    }
    printf("the max number is %d\n", max);

    return max;
}