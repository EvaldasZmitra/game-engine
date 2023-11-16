#include <elf_math.h>

int elf_math_min(int a, int b)
{
    return a > b ? b : a;
}

int elf_math_max(int a, int b)
{
    return a > b ? a : b;
}

int elf_math_divide_round_up(int numerator, int denominator)
{
    return (numerator + denominator - 1) / denominator;
}
