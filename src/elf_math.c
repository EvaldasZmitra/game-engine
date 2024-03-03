#include <elf_math.h>
#include <math.h>

inline void elf_math_vec3_subtract(float *c, const float *a, const float *b)
{
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
}

inline void elf_math_vec3_normalize(float *vec)
{
    float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    vec[0] = vec[0] / length;
    vec[1] = vec[1] / length;
    vec[2] = vec[2] / length;
}

inline void elf_math_view(
    float *matrix,
    float *camera,
    float *center,
    float *up)
{
    float forward[3];
    float right[3];
    elf_math_vec3_subtract(forward, center, camera);
    elf_math_vec3_cross(right, up, forward);
    elf_math_vec3_normalize(forward);
    elf_math_vec3_normalize(right);

    matrix[0] = right[0];
    matrix[1] = up[0];
    matrix[2] = -forward[0];

    matrix[4] = right[1];
    matrix[5] = up[1];
    matrix[6] = -forward[1];

    matrix[8] = right[2];
    matrix[9] = up[2];
    matrix[10] = -forward[2];

    matrix[12] = -(right[0] * camera[0] + right[1] * camera[1] + right[2] * camera[2]);
    matrix[13] = -(up[0] * camera[0] + up[1] * camera[1] + up[2] * camera[2]);
    matrix[14] = forward[0] * camera[0] + forward[1] * camera[1] + forward[2] * camera[2];
}

inline void elf_math_project(
    float *matrix,
    float fov,
    float aspectRatio,
    float near,
    float far)
{
    float focalLength = 1.0f / tan(fov * 0.5f);
    matrix[0] = focalLength / aspectRatio;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;

    matrix[4] = 0;
    matrix[5] = focalLength;
    matrix[6] = 0;
    matrix[7] = 0;

    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = (far + near) / (near - far);
    matrix[11] = (2.0f * far * near) / (near - far);

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = -1.0f;
    matrix[15] = 0;
}

inline int elf_math_min(int a, int b)
{
    return a > b ? b : a;
}

inline int elf_math_max(int a, int b)
{
    return a > b ? a : b;
}

inline int elf_math_divide_round_up(int numerator, int denominator)
{
    return (numerator + denominator - 1) / denominator;
}

inline void elf_math_quaternion_create(
    float *result,
    const float *axis,
    const float angle)
{
    float angle_sin = sin(angle / 2);
    result[0] = axis[0] * angle_sin;
    result[1] = axis[1] * angle_sin;
    result[2] = axis[2] * angle_sin;
    result[3] = cos(angle / 2);
}

inline void elf_math_quaternion_combine(
    float *result,
    const float *rotation1,
    const float *rotation2)
{
    result[0] = rotation1[0] * rotation2[3] + rotation1[1] * rotation2[2] - rotation1[2] * rotation2[1] + rotation1[3] * rotation2[0];
    result[1] = -rotation1[0] * rotation2[2] + rotation1[1] * rotation2[3] + rotation1[2] * rotation2[0] + rotation1[3] * rotation2[1];
    result[2] = rotation1[0] * rotation2[1] - rotation1[1] * rotation2[0] + rotation1[2] * rotation2[3] + rotation1[3] * rotation2[2];
    result[3] = -rotation1[0] * rotation2[0] - rotation1[1] * rotation2[1] - rotation1[2] * rotation2[2] + rotation1[3] * rotation2[3];
}

inline void elf_math_transform_create(
    float *matrix,
    const float *rotation,
    const float *position,
    const float *scale)
{
    // float ww = rotation[3] * rotation[3];
    // float xx = rotation[0] * rotation[0];
    // float yy = rotation[1] * rotation[1];
    // float zz = rotation[2] * rotation[2];

    // float xy = 2 * rotation[0] * rotation[1];
    // float xz = 2 * rotation[0] * rotation[2];
    // float yz = 2 * rotation[1] * rotation[2];

    // float wx = 2 * rotation[3] * rotation[0];
    // float wy = 2 * rotation[3] * rotation[1];
    // float wz = 2 * rotation[3] * rotation[2];

    // float r33 = ww + xx + yy + zz;
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = position[0];

    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = position[1];

    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 1;
    matrix[11] = position[2];

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

inline float elf_math_vec3_dot(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline float elf_math_vec4_dot(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

inline void elf_math_vec3_cross(float *result, const float *a, const float *b)
{
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[1] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

inline void elf_math_view_create(
    float *matrix,
    const float *position,
    const float *forward,
    const float *up,
    const float *right)
{
    float dx = elf_math_vec4_dot(position, right);
    float dy = elf_math_vec4_dot(position, up);
    float dz = elf_math_vec4_dot(position, forward);

    matrix[0] = right[0];
    matrix[1] = right[1];
    matrix[2] = right[2];
    matrix[3] = -dx;

    matrix[4] = up[0];
    matrix[5] = up[1];
    matrix[6] = up[2];
    matrix[7] = -dy;

    matrix[8] = -forward[0];
    matrix[9] = -forward[1];
    matrix[10] = -forward[2];
    matrix[11] = dz;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

inline void elf_math_matrix_4x4_multiply(
    float *result,
    const float *matrix1,
    const float *matrix2)
{
    result[0] = matrix1[0] * matrix2[0] + matrix1[1] * matrix2[1] + matrix1[2] * matrix2[2] + matrix1[3] * matrix2[3];
    result[1] = matrix1[0] * matrix2[4] + matrix1[1] * matrix2[5] + matrix1[2] * matrix2[6] + matrix1[3] * matrix2[7];
    result[2] = matrix1[0] * matrix2[8] + matrix1[1] * matrix2[9] + matrix1[2] * matrix2[10] + matrix1[3] * matrix2[11];
    result[3] = matrix1[0] * matrix2[12] + matrix1[1] * matrix2[13] + matrix1[2] * matrix2[14] + matrix1[3] * matrix2[15];

    result[4] = matrix1[4] * matrix2[0] + matrix1[5] * matrix2[1] + matrix1[6] * matrix2[2] + matrix1[7] * matrix2[3];
    result[5] = matrix1[4] * matrix2[4] + matrix1[5] * matrix2[5] + matrix1[6] * matrix2[6] + matrix1[7] * matrix2[7];
    result[6] = matrix1[4] * matrix2[8] + matrix1[5] * matrix2[9] + matrix1[6] * matrix2[10] + matrix1[7] * matrix2[11];
    result[7] = matrix1[4] * matrix2[12] + matrix1[5] * matrix2[13] + matrix1[6] * matrix2[14] + matrix1[7] * matrix2[15];

    result[8] = matrix1[8] * matrix2[0] + matrix1[9] * matrix2[1] + matrix1[10] * matrix2[2] + matrix1[11] * matrix2[3];
    result[9] = matrix1[8] * matrix2[4] + matrix1[9] * matrix2[5] + matrix1[10] * matrix2[6] + matrix1[11] * matrix2[7];
    result[10] = matrix1[8] * matrix2[8] + matrix1[9] * matrix2[9] + matrix1[10] * matrix2[10] + matrix1[11] * matrix2[11];
    result[11] = matrix1[8] * matrix2[12] + matrix1[9] * matrix2[13] + matrix1[10] * matrix2[14] + matrix1[11] * matrix2[15];

    result[12] = matrix1[12] * matrix2[0] + matrix1[13] * matrix2[1] + matrix1[14] * matrix2[2] + matrix1[15] * matrix2[3];
    result[13] = matrix1[12] * matrix2[4] + matrix1[13] * matrix2[5] + matrix1[14] * matrix2[6] + matrix1[15] * matrix2[7];
    result[14] = matrix1[12] * matrix2[8] + matrix1[13] * matrix2[9] + matrix1[14] * matrix2[10] + matrix1[15] * matrix2[11];
    result[15] = matrix1[12] * matrix2[12] + matrix1[13] * matrix2[13] + matrix1[14] * matrix2[14] + matrix1[15] * matrix2[15];
}

inline void elf_math_matrix_4x4_vec4_multiply(
    float *result,
    const float *matrix1,
    const float *vector)
{
    result[0] = matrix1[0] * vector[0] + matrix1[1] * vector[0] + matrix1[2] * vector[0] + matrix1[3] * vector[0];
    result[1] = matrix1[4] * vector[1] + matrix1[5] * vector[1] + matrix1[6] * vector[1] + matrix1[7] * vector[1];
    result[2] = matrix1[8] * vector[2] + matrix1[9] * vector[2] + matrix1[10] * vector[2] + matrix1[11] * vector[2];
    result[3] = matrix1[12] * vector[3] + matrix1[13] * vector[3] + matrix1[14] * vector[3] + matrix1[15] * vector[3];
}
