#include <elf_tranform.h>

void elf_transform(ElfTransform *transform, mat4 transform_matrix)
{
    mat4 translation, scale, rotation;

    glm_translate(translation, transform->position);
    elf_transform_rotation(rotation, transform->rotation);
    glm_scale(scale, transform->scale);

    glm_mul(translation, rotation, transform_matrix);
    glm_mul(transform_matrix, scale, transform_matrix);
}

void elf_transform_rotation(mat4 rotation, vec3 euler_angles)
{
    mat4 rotation_x;
    mat4 rotation_y;
    mat4 rotation_z;

    glm_rotate(rotation_x, euler_angles[0], (vec3){1, 0, 0});
    glm_rotate(rotation_y, euler_angles[1], (vec3){0, 1, 0});
    glm_rotate(rotation_z, euler_angles[2], (vec3){0, 0, 1});

    glm_mul(rotation_x, rotation_y, rotation);
    glm_mul(rotation, rotation_z, rotation);
}
