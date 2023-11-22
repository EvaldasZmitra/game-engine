#include <stdio.h>
#include <assert.h>
#include <elf_math.h>

#undef main

void test()
{
    float result1[4];
    float result2[4];
    elf_math_create_rotation(result1, (float[]){0, 0, 1}, 3.14);
    elf_math_combine_rotation(result2, (float[]){1, 0, 0, 0}, (float[]){0, 0, 1, 0});
}

int main()
{
    printf("Tests started");
    test();
    return 0;
}
