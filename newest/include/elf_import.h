#ifndef ELF_IMPORTER_H
#define ELF_IMPORTER_H

#include <GL/glew.h>
#include <assimp/mesh.h>
#include <cglm/cglm.h>

typedef struct elf_anim_bone
{
    vec3 position;
    vec4 position;
    vec3 scale;
} elf_anim_bone;

typedef struct elf_anim
{
    GLchar *name;
    GLdouble ticks_per_second;
    GLdouble duration;
    elf_anim_bone *bones;
};

typedef struct elf_mdl
{
    GLchar *name;
    GLfloat *vertices;
    GLfloat *texture_coords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
    GLuint *texture_coord_components[AI_MAX_NUMBER_OF_TEXTURECOORDS];
    GLfloat *colors[AI_MAX_NUMBER_OF_COLOR_SETS];
    GLfloat *normals;
    GLfloat *tangents;
    GLfloat *bit_tangents;
    GLfloat *weights;
    GLint *bone_ids;
    GLuint *indices;
    GLuint num_vertices;
};

typedef struct elf_shd
{
    GLchar *name;
    GLenum format;
    GLubyte *binary_data;
    GLint binary_length;
} elf_shd;

typedef struct elf_bone
{
    GLchar *name;
    GLint id;
    GLint parent_id;
    mat4 offset;
} elf_bone;

typedef struct elf_skel
{
    GLchar *name;
    elf_bone *bones;
};

typedef struct elf_tex
{
    GLchar *name;
    unsigned int height;
    unsigned int width;
    unsigned int linear_size;
    unsigned int mip_map_count;
    unsigned int four_cc;
    unsigned char *buffer;
};

void elf_import_mdl();
void elf_import_tex();
void elf_import_shd();

#endif
