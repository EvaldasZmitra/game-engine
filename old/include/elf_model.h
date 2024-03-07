#ifndef ELF_MODEL_H
#define ELF_MODEL_H
#include <elf_material.h>
#include <elf_transform.h>

enum ModelLocation
{
    VERTICES,
    TEXTURE_COORDS_0,
    TEXTURE_COORDS_1,
    TEXTURE_COORDS_2,
    TEXTURE_COORDS_3,
    TEXTURE_COORDS_4,
    TEXTURE_COORDS_5,
    TEXTURE_COORDS_6,
    TEXTURE_COORDS_7,
    NORMALS,
    TANGENTS,
    BIT_TANGENTS,
    BONE_IDS,
    BONE_WEIGHTS,
    INDICES,
    NUM_BUFFERS
};

typedef struct ElfModel
{
    unsigned int vao;
    unsigned int buffers[NUM_BUFFERS];
    unsigned int num_indices;
    ElfTransform transform;
} ElfModel;

typedef struct ElfModels
{
    unsigned int num_models;
    ElfModel *models;
} ElfModels;

ElfModels elf_model_load(const char *path);
void elf_model_free(ElfModel *model);

void elf_model_draw_instanced(ElfModel *model, int num_instances);
void elf_model_draw(ElfModel *model, unsigned int shader);
void elf_model_use_material(elf_material *elf_material);
void elf_model_bind_shader(ElfModel *model, unsigned int shader);

#endif