#ifndef ELF_MATERIAL_H
#define ELF_MATERIAL_H

#include "elf_texture.h"

typedef struct elf_material
{
    unsigned int num_textures;
    elf_texture *textures;
    unsigned int shader_id;
} elf_material;

#endif