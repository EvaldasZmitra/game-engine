#include <stdio.h>
#include <assert.h>
#include <elf_math.h>
#include "elf_math_tests.h"

int main()
{
    elf_math_vec3_subtract_test();
    elf_math_vec3_normalize_test();
    return 0;
}
