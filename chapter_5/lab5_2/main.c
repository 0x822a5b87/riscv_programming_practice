#include "stdio.h"

extern int cmp(int l, int r);

int main(int argc, char const *argv[])
{
    int l = 10;
    int r = 20;

    int number = cmp(l, r);
    printf("max number is %d\n", number);
    
    return 0;
}
