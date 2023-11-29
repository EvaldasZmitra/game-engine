#ifndef ELF_MATH_H
#define ELF_MATH_H

inline int elf_math_min(int a, int b);

inline int elf_math_max(int a, int b);

inline int elf_math_divide_round_up(int numerator, int denominator);

inline float elf_math_vec3_dot(const float *a, const float *b);

inline void elf_math_vec3_cross(float *result, const float *a, const float *b);

inline void elf_math_matrix_4x4_multiply(
    float *result,
    const float *matrix1,
    const float *matrix2);

inline void elf_math_quaternion_create(
    float *result,
    const float *axis,
    const float angle);

inline void elf_math_quaternion_combine(
    float *result,
    const float *rotation1,
    const float *rotation2);

inline void elf_math_transform_create(
    float *matrix,
    const float *rotation,
    const float *position,
    const float *scale);

inline void elf_math_view_create(
    float *matrix,
    const float *position,
    const float *forward,
    const float *up,
    const float *right);

#endif