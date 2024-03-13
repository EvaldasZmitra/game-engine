#ifndef ELF_LOAD_H
#define ELF_LOAD_H

#include <glib/ghash.h>
#include <GL/glew.h>
#include "elf_animation.h"
#include "elf_bone.h"

typedef struct elf_loader
{
    GHashTable *cache;
    const GLchar *res_path;
} elf_loader;

GLuint elf_load_res_buffer(const elf_loader *loader, GLchar *name, GLenum target, GLenum usage);
GLuint elf_load_res_texture(const elf_loader *loader, GLchar *name);
elf_bone *elf_load_res_skeleton(const elf_loader *loader, GLchar *name);
elf_animation elf_load_res_animation(const elf_loader *loader, GLchar *name);
GLuint elf_load_res_vao(const elf_loader *loader, GLchar *name);
GLuint elf_load_res_shader(const elf_loader *loader, GLchar *name);

GLuint elf_load_comp(const GLchar *src_code);
GLuint elf_load_vert(const GLchar *src_code);
GLuint elf_load_tess_ctrl(const GLchar *src_code);
GLuint elf_load_tess_eval(const GLchar *src_code);
GLuint elf_load_geom(const GLchar *src_code);
GLuint elf_load_frag(const GLchar *src_code);

#endif