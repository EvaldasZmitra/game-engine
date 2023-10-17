#ifndef ELF_RENDER_OBJECT
#define ELF_RENDER_OBJECT

#include <elf_shader.h>

typedef struct ElfRenderObject
{
    ElfShader *shader;
    unsigned int count;
} ElfRenderObject;

void elf_render_object_render(ElfRenderObject *object);

#endif