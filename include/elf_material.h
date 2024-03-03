#ifndef ELF_MATERIAL_H
#define ELF_MATERIAL_H

#include "elf_texture.h"

typedef struct ElfMaterial
{
    unsigned int num_textures;
    ElfTexture *textures;
    unsigned int shader;
} ElfMaterial;

#endif