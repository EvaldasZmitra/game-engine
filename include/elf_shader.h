#ifndef ELF_SHADER_H
#define ELF_SHADER_H

#include <GL/glew.h>

typedef struct ElfShader
{
    GLuint id;
    GLuint vao_id;
    GLuint vbo_id;

} ElfShader;

ElfShader elf_shader_create(const char *vertex_code, const char *fragment_code);
void elf_shader_use(const ElfShader *shader);
void elf_shader_free(const ElfShader *shader);
void elf_shader_add_vec3_data(ElfShader *shader, float *vectors, unsigned int num_vectors, unsigned int index);

#endif