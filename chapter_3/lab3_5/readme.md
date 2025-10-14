# readme

## source code

```c
unsigned long compare_and_return(unsigned long a, unsigned long b)
{
    if (a >= b)
        return 0;
    else
        return 0xffffffffffffffff;
}
```

## asm