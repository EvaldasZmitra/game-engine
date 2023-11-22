#ifndef ELF_MATH_H
#define ELF_MATH_H

#include <cglm/cglm.h>

int elf_math_min(int a, int b);
int elf_math_max(int a, int b);
int elf_math_divide_round_up(int numerator, int denominator);
void elf_math_create_rotation(float *result, float *axis, float angle);
void elf_math_combine_rotation(float *result, float *rotation1, float *rotation2);
void elf_math_create_transform(float *matrix, float *rotation, float *position, float *scale);

#endif