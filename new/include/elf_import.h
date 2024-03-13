#ifndef ELF_IMPORT_H
#define ELF_IMPORT_H

#include <GL/glew.h>
#include <assimp/mesh.h>
#include <cglm/cglm.h>

void elf_import_model(const char *path, const char *replace, unsigned int flags);
void elf_import_shader(
    const char *comp,
    const char *vert,
    const char *tess_ctrl,
    const char *tess_eval,
    const char *geom,
    const char *frag);

#endif
