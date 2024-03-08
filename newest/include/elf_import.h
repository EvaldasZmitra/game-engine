#ifndef ELF_IMPORTER_H
#define ELF_IMPORTER_H

#include <GL/glew.h>
#include <assimp/mesh.h>
#include <cglm/cglm.h>
#include <elf_str.h>

typedef struct elf_anim_bone
{
    vec3 position;
    vec4 rotation;
    vec3 scale;
} elf_anim_bone;

typedef struct elf_anim
{
    GLchar *name;
    GLdouble ticks_per_second;
    GLdouble duration;
    elf_anim_bone *bones;
} elf_anim;

typedef struct elf_mesh
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
} elf_mesh;

typedef struct elf_mdl
{
    elf_mesh *meshes;
} elf_mdl;

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
} elf_skel;

typedef struct elf_tex
{
    GLchar *name;
    unsigned int height;
    unsigned int width;
    unsigned int linear_size;
    unsigned int mip_map_count;
    unsigned int four_cc;
    unsigned char *buffer;
} elf_tex;

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
