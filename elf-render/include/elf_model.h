#ifndef ELF_MODEL_H
#define ELF_MODEL_H

enum ModelLocation
{
    VERTICES,
    COLORS,
    TEXTURE_COORDS,
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

} ElfModel;

ElfModel elf_model_load(const char *path);
void elf_model_free(ElfModel *model);

void elf_model_draw_instanced(ElfModel *model, int num_instances);
void elf_model_draw(ElfModel *model);

#endif