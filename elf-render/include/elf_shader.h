#ifndef ELF_SHADER_H
#define ELF_SHADER_H

#include <GL/glew.h>

unsigned int elf_shader_create(const char *vertex_code, const char *fragment_code);
void elf_shader_destroy(unsigned int shader_id);
void elf_shader_use(unsigned int shader_id);
void elf_shader_set_matrix4x4(unsigned int shader_id, const char *name, float *data);
void elf_shader_set_vec4(unsigned int shader_id, const char *name, float *data);
void elf_shader_set_vec3(unsigned int shader_id, const char *name, float *data);
void elf_shader_set_vec2(unsigned int shader_id, const char *name, float *data);

#endif