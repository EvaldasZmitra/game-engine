#include <elf_import.h>
#include <glib.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/anim.h>
#include <glib.h>

typedef struct aiMesh mesh;
typedef struct aiScene scene;
typedef struct aiAnimation animation;
typedef struct aiMaterial material;
typedef struct aiMesh mesh;
typedef struct aiSkeleton skeleton;
typedef struct aiTexture texture;
typedef struct aiNodeAnim node_anim;
typedef struct aiMeshAnim mesh_anim;
typedef struct aiMeshMorphAnim mesh_morph_anim;
typedef struct aiVectorKey vector_key;
typedef struct aiQuatKey quat_key;
typedef struct aiMeshMorphKey mesh_morph_key;
typedef struct aiBone bone;
typedef struct aiSkeletonBone skeletal_bone;
typedef struct aiVertexWeight weight;
typedef struct aiMaterialProperty property;
typedef struct aiFace face;

void elf_import_model(
    const char *path,
    const char *replace,
    unsigned int flags)
{
}

void elf_import_shader(const char *path)
{
}
