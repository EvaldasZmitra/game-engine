#include <stdio.h>
#include <math.h>

void create_vec3_str(char *result, float *value)
{
    sprintf(result, "(%.2f, %.2f, %.2f)", value[0], value[1], value[2]);
}

void assert_vec3_equals(float *expected, float *actual)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (expected[i] != actual[i])
        {
            char expected_str[20];
            char actual_str[20];
            create_vec3_str(expected_str, expected);
            create_vec3_str(actual_str, actual);
            printf("Expected %s, but found %s\n", expected_str, actual_str);
        }
    }
}

void assert_vec3_approx(float *expected, float *actual, float max_diff)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (fabs(expected[i] - actual[i]) > max_diff)
        {
            printf("Max diff exceeded.");
        }
    }
}
