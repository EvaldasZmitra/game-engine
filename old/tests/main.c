#include <stdio.h>
#include <assert.h>
#include <elf_math.h>
#include <cglm/cglm.h>
#include <cglm/project.h>
#include "elf_math_tests.h"

void my_print(float *mat)
{
    for (size_t i = 0; i < 16; i++)
    {
        printf("%.2f ", mat[i]);
        if (i % 4 == 3)
        {
            printf("\n");
        }
    }
    printf("\n");
}

int main()
{

    // glm_mat4_identity(model);

    // elf_math_vec3_subtract_test();
    // elf_math_vec3_normalize_test();

    // float projection_matrix[16];
    // elf_math_project(
    //     projection_matrix,
    //     60,
    //     1,
    //     0.1f,
    //     100.0f);

    // float position[3] = {0, 0, 0};
    // float scale[3] = {0.5, 0.5, 0.5};
    // float rotation[3] = {0, 0, 0};
    // float model_matrix[16];
    // elf_math_transform_create(
    //     model_matrix,
    //     rotation,
    //     position,
    //     scale);

    // float view_matrix[16];
    // float camera_position[3] = {0, 0, -5};
    // float center[3] = {0, 0, 0};
    // float up[3] = {0, 1, 0};
    // elf_math_view(
    //     view_matrix,
    //     camera_position,
    //     center,
    //     up);

    // float vp[16];
    // float mvp[16];
    // elf_math_matrix_4x4_multiply(vp, projection_matrix, view_matrix);
    // elf_math_matrix_4x4_multiply(mvp, vp, model_matrix);

    // float rr4[4];
    // elf_math_matrix_4x4_vec4_multiply(rr4, mvp, (float[]){-1, 0, 0, 1});
    // for (size_t i = 0; i < 16; i++)
    // {
    //     printf("%.2f ", mvp[i]);
    //     if (i % 4 == 3)
    //     {
    //         printf("\n");
    //     }
    // }

    // printf("\n");
    // for (size_t i = 0; i < 4; i++)
    // {
    //     printf("%.2f ", rr4[i]);
    // }

    return 0;
}
