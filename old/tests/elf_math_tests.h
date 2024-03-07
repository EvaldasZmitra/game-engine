#include <stdio.h>
#include <elf_math.h>
#include "validate.h"

void elf_math_vec3_subtract_test()
{
    float result[3];
    float a[] = {0, 1, 2};
    float b[] = {1, 1, 1};

    elf_math_vec3_subtract(result, a, b);

    assert_vec3_equals((float[]){-1.0f, 0.0f, 1.0f}, result);
}

void elf_math_vec3_normalize_test()
{
    float result[3] = {1, 1, 1};

    elf_math_vec3_normalize(result);

    assert_vec3_approx((float[]){0.58f, 0.58f, 0.58f}, result, 0.1f);
}

void elf_math_project_test()
{
}
