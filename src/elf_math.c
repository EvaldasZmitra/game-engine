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

void elf_math_create_rotation(float *result, float *axis, float angle)
{
    float angle_sin = sin(angle / 2);
    result[0] = axis[0] * angle_sin;
    result[1] = axis[1] * angle_sin;
    result[2] = axis[2] * angle_sin;
    result[3] = cos(angle / 2);
}

void elf_math_combine_rotation(float *result, float *rotation1, float *rotation2)
{
    result[0] = rotation1[0] * rotation2[3] + rotation1[1] * rotation2[2] - rotation1[2] * rotation2[1] + rotation1[3] * rotation2[0];
    result[1] = -rotation1[0] * rotation2[2] + rotation1[1] * rotation2[3] + rotation1[2] * rotation2[0] + rotation1[3] * rotation2[1];
    result[2] = rotation1[0] * rotation2[1] - rotation1[1] * rotation2[0] + rotation1[2] * rotation2[3] + rotation1[3] * rotation2[2];
    result[3] = -rotation1[0] * rotation2[0] - rotation1[1] * rotation2[1] - rotation1[2] * rotation2[2] + rotation1[3] * rotation2[3];
}

void elf_math_create_transform(float *matrix, float *rotation, float *position, float *scale)
{
    float ww = rotation[3] * rotation[3];
    float xx = rotation[0] * rotation[0];
    float yy = rotation[1] * rotation[1];
    float zz = rotation[2] * rotation[2];

    float xy = 2 * rotation[0] * rotation[1];
    float xz = 2 * rotation[0] * rotation[2];
    float yz = 2 * rotation[1] * rotation[2];

    float wx = 2 * rotation[3] * rotation[0];
    float wy = 2 * rotation[3] * rotation[1];
    float wz = 2 * rotation[3] * rotation[2];

    float r33 = ww + xx + yy + zz;

    matrix[0] = (ww + xx - yy - zz) * scale[0];
    matrix[1] = (xy + wz) * scale[1];
    matrix[2] = (xz - wy) * scale[2];
    matrix[3] = position[0] * r33;

    matrix[4] = (xy - wz) * scale[0];
    matrix[5] = (ww + yy - xx - zz) * scale[1];
    matrix[6] = (yz + wx) * scale[2];
    matrix[7] = position[1] * r33;

    matrix[8] = (xz + wy) * scale[0];
    matrix[9] = (yz - wx) * scale[1];
    matrix[10] = (ww + zz - xx - yy) * scale[2];
    matrix[11] = position[2] * r33;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = r33;
}
