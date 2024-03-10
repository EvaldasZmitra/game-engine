#ifndef ELF_IMPORTER_H
#define ELF_IMPORTER_H

#include <GL/glew.h>
#include <assimp/mesh.h>
#include <cglm/cglm.h>
#include <elf_str.h>

void elf_import_mdl();
void elf_import_tex();
void elf_import_shd();
void elf_load_model(
    const elf_str *path,
    const elf_str *replace,
    unsigned int flags);
void elf_load_program(
    const elf_str *path,
    const GLuint comp,
    const GLuint vert,
    const GLuint tess_ctrl,
    const GLuint tess_eval,
    const GLuint geom,
    const GLuint frag);
GLuint elf_load_comp(const GLchar *src_code);
GLuint elf_load_vert(const GLchar *src_code);
GLuint elf_load_tess_ctrl(const GLchar *src_code);
GLuint elf_load_tess_eval(const GLchar *src_code);
GLuint elf_load_geom(const GLchar *src_code);
GLuint elf_load_frag(const GLchar *src_code);
#endif
