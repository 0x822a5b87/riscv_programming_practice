#include "stdio.h"
#include "atomic.h"

int main(int argc, char const *argv[])
{
    int a = 10;
    printf("a = %d\n", a);
    my_atomic_add(20, &a);
    //printf("a = %d\n", a);
    return 0;
}