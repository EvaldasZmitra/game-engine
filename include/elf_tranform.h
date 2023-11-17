#ifndef ELF_TRANSFORM_H
#define ELF_TRANSFORM_H

#include <cglm/cglm.h>

typedef struct ElfTransform
{
    vec3 position;
    vec4 rotation;
    vec3 scale;
} ElfTransform;

void elf_transform(ElfTransform *transform);
void elf_transform_rotation(mat4 rotation, vec3 euler_angles);

#endif