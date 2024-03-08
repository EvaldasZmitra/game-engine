#include <elf_import.h>
#include <glib.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/anim.h>
#include <elf_str.h>
#include <elf_io.h>
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

GLuint elf_load_shader(const GLchar *src_code, GLenum type)
{
    GLuint vertex_shader_id = glCreateShader(type);
    glShaderSource(vertex_shader_id, 1, &src_code, NULL);
    glCompileShader(vertex_shader_id);

    GLuint result = GL_FALSE;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLint error_length;
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &error_length);
        GLchar *error_msg = malloc(sizeof(GLchar) * error_length);
        glGetShaderInfoLog(vertex_shader_id, error_length, NULL, error_msg);
        fprintf(stderr, "%s\n", error_msg);
        return -1;
    }
    return vertex_shader_id;
}

GLuint elf_load_comp(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_COMPUTE_SHADER);
}

GLuint elf_load_vert(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_VERTEX_SHADER);
}

GLuint elf_load_tess_ctrl(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_TESS_CONTROL_SHADER);
}

GLuint elf_load_tess_eval(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_TESS_EVALUATION_SHADER);
}

GLuint elf_load_geom(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_GEOMETRY_SHADER);
}

GLuint elf_load_frag(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_FRAGMENT_SHADER);
}

void elf_load_program(
    const elf_str *path,
    const GLuint comp,
    const GLuint vert,
    const GLuint tess_ctrl,
    const GLuint tess_eval,
    const GLuint geom,
    const GLuint frag)
{
    GLuint program_id = glCreateProgram();
    if (comp != NULL)
    {
        glAttachShader(program_id, comp);
    }
    if (vert != NULL)
    {
        glAttachShader(program_id, vert);
    }
    if (tess_ctrl != NULL)
    {
        glAttachShader(program_id, tess_ctrl);
    }
    if (tess_eval != NULL)
    {
        glAttachShader(program_id, tess_eval);
    }
    if (geom != NULL)
    {
        glAttachShader(program_id, geom);
    }
    if (frag != NULL)
    {
        glAttachShader(program_id, frag);
    }
    glLinkProgram(program_id);
    GLint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLint error_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &error_length);
        GLchar *error_msg = malloc(sizeof(GLchar) * error_length);
        glGetProgramInfoLog(program_id, error_length, NULL, error_msg);
        printf("%s\n", error_msg);
    }
    GLint size;
    GLenum format;
    glGetProgramiv(program_id, GL_PROGRAM_BINARY_LENGTH, &size);
    char *data = malloc(size * sizeof(GLchar));
    glGetProgramBinary(program_id, size, NULL, &format, data);
    elf_write_buffer(path, &format, sizeof(GLenum));
    elf_append_buffer(path, data, size);
}

void elf_load_model(
    const elf_str *path,
    const elf_str *replace,
    unsigned int flags)
{
    const scene *scene = aiImportFile(path->chars, flags);
    int current_bone_id = 0;
    GHashTable *bones = g_hash_table_new(g_str_hash, g_str_equal);
    if (scene->mNumMeshes > 0)
    {
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            mesh *mesh = scene->mMeshes[i];
            for (unsigned int y = 0; y < mesh->mNumBones; y++)
            {
                bone *bone = mesh->mBones[y];
                if (!g_hash_table_contains(bones, bone->mName.data))
                {
                    g_hash_table_insert(
                        bones,
                        bone->mName.data,
                        GINT_TO_POINTER(current_bone_id));
                    current_bone_id++;
                }
            }
        }
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            mesh *mesh = scene->mMeshes[i];
            elf_str mdl_name = elf_str_init_null("/mdl/", 1024);
            elf_str mesh_name = elf_str_init(mesh->mName.data, mesh->mName.length);
            elf_str mdl_path = elf_str_add(replace, &mdl_name);
            elf_str this_mdl_path = elf_str_add(&mdl_path, &mesh_name);

            // Vertices
            elf_str vertex_name = elf_str_init_null("/vertex", 1024);
            elf_str this_vertex_path = elf_str_add(&this_mdl_path, &vertex_name);
            elf_write_buffer(
                &this_vertex_path,
                &mesh->mVertices[0].x,
                mesh->mNumVertices * 3 * sizeof(float));

            // Normals
            elf_str normal_name = elf_str_init_null("/normal", 1024);
            elf_str this_normal_path = elf_str_add(&this_mdl_path, &normal_name);
            elf_write_buffer(
                &this_normal_path,
                &mesh->mNormals[0].x,
                mesh->mNumVertices * 3 * sizeof(float));

            // Tangents
            elf_str tangent_name = elf_str_init_null("/tangent", 1024);
            elf_str this_tangent_path = elf_str_add(&this_mdl_path, &tangent_name);
            elf_write_buffer(
                &this_tangent_path,
                &mesh->mTangents[0].x,
                mesh->mNumVertices * 3 * sizeof(float));

            // Bit tangents
            elf_str bit_tangent_name = elf_str_init_null("/bit_tangent", 1024);
            elf_str this_bit_tangent_path = elf_str_add(&this_mdl_path, &bit_tangent_name);
            elf_write_buffer(
                &this_bit_tangent_path,
                &mesh->mBitangents[0].x,
                mesh->mNumVertices * 3 * sizeof(float));

            // Faces
            elf_str index_name = elf_str_init_null("/index", 1024);
            elf_str this_index_path = elf_str_add(&this_mdl_path, &index_name);
            unsigned int indices_size = mesh->mNumFaces * 3 * sizeof(unsigned int);
            unsigned int *indices = malloc(indices_size);
            for (unsigned int y = 0; y < mesh->mNumFaces; y++)
            {
                face *face = &mesh->mFaces[y];
                for (unsigned int u = 0; u < face->mNumIndices; u++)
                {
                    indices[y * 3 + u] = face->mIndices[u];
                }
            }
            elf_write_buffer(&this_index_path, indices, indices_size);
            free(indices);

            // Tex Coords
            for (unsigned int u = 0; u < AI_MAX_NUMBER_OF_TEXTURECOORDS; u++)
            {
                if (mesh->mTextureCoords[u] != NULL)
                {
                    unsigned int num_components = mesh->mNumUVComponents[u];
                    unsigned int coords_size = mesh->mNumUVComponents[u] * mesh->mNumVertices * sizeof(GLfloat);
                    GLfloat *coords = malloc(coords_size);
                    for (unsigned int y = 0; y < mesh->mNumVertices; y++)
                    {
                        for (unsigned int v = 0; v < num_components; v++)
                        {
                            if (v == 0)
                            {
                                coords[y * num_components] = mesh->mTextureCoords[u][y].x;
                            }
                            if (v == 1)
                            {
                                coords[y * num_components + 1] = mesh->mTextureCoords[u][y].y;
                            }
                            if (v == 2)
                            {
                                coords[y * num_components + 2] = mesh->mTextureCoords[u][y].z;
                            }
                        }
                    }
                    elf_str tex_name = elf_str_init_null("/tex", 1024);
                    char str[10];
                    sprintf(str, "%d", u);
                    elf_str tex_ind = elf_str_init_null(str, 1024);
                    elf_str tex_full = elf_str_add(&tex_name, &tex_ind);

                    elf_str this_tex_path = elf_str_add(&this_mdl_path, &tex_full);
                    elf_write_buffer(&this_tex_path, coords, coords_size);
                    elf_str_free(&this_tex_path);
                    elf_str_free(&tex_ind);
                    elf_str_free(&tex_full);
                    elf_str_free(&tex_name);
                }
            }

            // Bones
            unsigned int bone_size = mesh->mNumVertices * sizeof(GLfloat) * 4;
            GLfloat *bone_weights = malloc(bone_size);
            GLuint *bone_ids = malloc(bone_size);
            for (unsigned int y = 0; y < mesh->mNumBones; y++)
            {
                bone *bone = &mesh->mBones[y];
                if (g_hash_table_contains(bones, bone->mName.data))
                {
                    GLint bone_id = GPOINTER_TO_INT(g_hash_table_lookup(bones, bone->mName.data));
                    for (unsigned int u = 0; u < bone->mNumWeights; u++)
                    {
                        weight *weight = &bone->mWeights[u];
                        for (unsigned v = 0; v < 4; v++)
                        {
                            unsigned int ind = weight->mVertexId * 4 + v;
                            if (bone_ids[ind] == -1)
                            {
                                bone_weights[ind] = weight->mVertexId;
                                bone_ids[ind] = bone_id;
                                break;
                            }
                        }
                    }
                }
            }
            elf_str bone_weight_name = elf_str_init_null("/bone_weight", 1024);
            elf_str this_bone_weight_path = elf_str_add(&this_mdl_path, &bone_weight_name);
            elf_write_buffer(
                &this_bone_weight_path,
                bone_weights,
                bone_size);
            elf_str bone_id_name = elf_str_init_null("/bone_id", 1024);
            elf_str this_bone_id_path = elf_str_add(&this_mdl_path, &bone_id_name);
            elf_write_buffer(
                &this_bone_id_path,
                bone_ids,
                bone_size);

            elf_str_free(&mdl_name);
            elf_str_free(&mesh_name);
            elf_str_free(&mdl_path);
            elf_str_free(&this_mdl_path);
            elf_str_free(&vertex_name);
            elf_str_free(&this_vertex_path);
            elf_str_free(&this_normal_path);
            elf_str_free(&this_bit_tangent_path);
            elf_str_free(&this_tangent_path);
            elf_str_free(&this_index_path);
        }
    }
    for (unsigned int i = 0; i < scene->mNumTextures; i++)
    {
        texture *texture = scene->mTextures[i];
        elf_str internal_path = elf_str_init(
            texture->mFilename.data,
            texture->mFilename.length);

        elf_str tex_str = elf_str_init_null("/tex", 1024);
        elf_str tex_path = elf_str_add(replace, &tex_str);
        elf_str elf_str = elf_str_replace_path(&internal_path, &tex_path);
        elf_write_buffer(&elf_str, texture->pcData, texture->mWidth);
        elf_str_free(&internal_path);
        elf_str_free(&tex_str);
        elf_str_free(&tex_path);
        elf_str_free(&elf_str);
    }
    // for (unsigned int i = 0; i < scene->mNumAnimations; i++)
    // {
    //     animation *animation = &scene->mAnimations[i];
    //     animation->mTicksPerSecond;
    //     animation->mDuration;
    //     animation->mName;
    //     for (unsigned int y = 0; y < animation->mNumChannels; y++)
    //     {
    //         node_anim *channel = animation->mChannels[y];
    //         channel->mNodeName;
    //         channel->mPostState;
    //         channel->mPreState;
    //         for (unsigned int u; u < channel->mNumPositionKeys; u++)
    //         {
    //             vector_key *positionKey = &channel->mPositionKeys[u];
    //             positionKey->mTime;
    //             positionKey->mValue;
    //         }
    //         for (unsigned int u; u < channel->mNumRotationKeys; u++)
    //         {
    //             quat_key *rotation_key = &channel->mRotationKeys[u];
    //             rotation_key->mTime;
    //             rotation_key->mValue;
    //         }
    //         for (unsigned int u; u < channel->mNumScalingKeys; u++)
    //         {
    //             vector_key *scalingKey = &channel->mScalingKeys[u];
    //             scalingKey->mTime;
    //             scalingKey->mValue;
    //         }
    //     }
    //     for (unsigned int y = 0; y < animation->mNumMeshChannels; y++)
    //     {
    //         mesh_anim *mesh_channel = &animation->mMeshChannels[y];
    //         mesh_channel->mName;
    //         for (unsigned int u; u < mesh_channel->mNumKeys; u++)
    //         {
    //             mesh_morph_key *key = &mesh_channel->mKeys[u];
    //             key->mTime;
    //             for (unsigned int v = 0; v < key->mNumValuesAndWeights; v++)
    //             {
    //                 key->mValues[v];
    //                 key->mWeights[v];
    //             }
    //         }
    //     }
    //     for (unsigned int y = 0; y < animation->mNumMorphMeshChannels; y++)
    //     {
    //         mesh_morph_anim *morph_mesh_channel = &animation->mMorphMeshChannels[y];
    //         morph_mesh_channel->mName;
    //         for (unsigned int u; u < morph_mesh_channel->mNumKeys; u++)
    //         {
    //             mesh_morph_key *key = &morph_mesh_channel->mKeys[u];
    //             key->mTime;
    //             for (unsigned int v = 0; v < key->mNumValuesAndWeights; v++)
    //             {
    //                 key->mValues[v];
    //                 key->mWeights[v];
    //             }
    //         }
    //     }
    // }
}
