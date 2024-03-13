#ifndef ELF_BONE_H
#define ELF_BONE_H

#include <cglm/cglm.h>

typedef struct elf_bone
{
    unsigned int parent;
    mat4 offset;
} elf_bone;

#endif