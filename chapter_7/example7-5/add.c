int add(int i, int j)
{
    int res = 0;

    asm volatile (
    "add %[result], %[input_i], %[input_j]"         // res = i + j
    : [result] "=r" (res)
    : [input_i] "r" (i), [input_j] "r" (j)
    );

    return res;
}