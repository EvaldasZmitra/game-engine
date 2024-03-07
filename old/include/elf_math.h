#ifndef ELF_MATH_H
#define ELF_MATH_H

void elf_math_vec3_normalize(float *vec);

int elf_math_min(int a, int b);

int elf_math_max(int a, int b);

int elf_math_divide_round_up(int numerator, int denominator);

void elf_math_vec3_subtract(float *c, const float *a, const float *b);

float elf_math_vec3_dot(const float *a, const float *b);

float elf_math_vec4_dot(const float *a, const float *b);

void elf_math_vec3_cross(float *result, const float *a, const float *b);

void elf_math_matrix_4x4_multiply(
    float *result,
    const float *matrix1,
    const float *matrix2);

void elf_math_quaternion_create(
    float *result,
    const float *axis,
    const float angle);

void elf_math_quaternion_combine(
    float *result,
    const float *rotation1,
    const float *rotation2);

void elf_math_transform_create(
    float *matrix,
    const float *rotation,
    const float *position,
    const float *scale);

void elf_math_view_create(
    float *matrix,
    const float *position,
    const float *forward,
    const float *up,
    const float *right);

void elf_math_project(
    float *matrix,
    float fov,
    float aspectRatio,
    float near,
    float far);

void elf_math_view(
    float *matrix,
    float *camera,
    float *center,
    float *up);

void elf_math_matrix_4x4_vec4_multiply(
    float *result,
    const float *matrix1,
    const float *vector);
#endif