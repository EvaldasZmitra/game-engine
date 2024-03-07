#include <GL/glew.h>
#include <assimp/mesh.h>

typedef struct elf_mesh
{
    GLchar *name;
    GLuint num_vertices;
    GLfloat *vertices;
    GLfloat *texture_coords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
    GLfloat *colors[AI_MAX_NUMBER_OF_COLOR_SETS];
    GLfloat *normals;
    GLfloat *tangents;
    GLfloat *bit_tangents;
    GLfloat *weights;
    GLint *bone_ids;
    GLuint *indices;
} elf_mesh;

typedef struct elf_bone
{
    GLchar *name;
    GLint id;
    GLint parent_id
} elf_bone;

typedef struct elf_skeleton
{
    GLchar *name;
    elf_bone *bones;
} elf_skeleton;

typedef struct elf_model
{
    elf_mesh *meshes;
    elf_skeleton *skeletons;
} elf_model;

elf_model elf_load_model(const char *path, unsigned int flags);
