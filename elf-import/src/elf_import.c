#include <elf_import.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <glib.h>
#include <GL/glew.h>
#include <sys/stat.h>

typedef struct aiScene Scene;
typedef struct aiMesh Mesh;
typedef struct aiFace Face;
typedef struct aiBone Bone;
typedef struct aiVertexWeight Weight;
typedef struct aiTexture Texture;

void elf_import_model(
    const char *model_path,
    const char *res_path,
    unsigned int flags)
{
    const Scene *scene = aiImportFile(model_path, flags);
    if (scene != NULL)
    {
        char path[2048];
        GHashTable *bones = g_hash_table_new(g_str_hash, g_str_equal);

        unsigned int current_bone_index = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            Mesh *mesh = scene->mMeshes[i];
            if (mesh->mNumBones)
            {
                for (unsigned int y = 0; y < mesh->mNumBones; y++)
                {
                    Bone *bone = mesh->mBones[y];
                    if (!g_hash_table_contains(bones, bone->mName.data))
                    {
                        g_hash_table_insert(bones, bone->mName.data, GUINT_TO_POINTER(current_bone_index));
                        current_bone_index++;
                    }
                }
            }
        }
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            Mesh *mesh = scene->mMeshes[i];
            sprintf(path, "%s/mdl", res_path);
            mkdir(path);
            sprintf(path, "%s/mdl/%s%d", res_path, mesh->mName.data, i);
            FILE *file = fopen(path, "wb");
            unsigned int zero = 0;

            if (mesh->mVertices != NULL)
            {
                fwrite(&mesh->mNumVertices, sizeof(unsigned int), 1, file);
            }
            else
            {
                fwrite(&zero, sizeof(unsigned int), 1, file);
            }

            if (mesh->mNormals != NULL)
            {
                fwrite(&mesh->mNumVertices, sizeof(unsigned int), 1, file);
            }
            else
            {
                fwrite(&zero, sizeof(unsigned int), 1, file);
            }

            if (mesh->mTangents != NULL)
            {
                fwrite(&mesh->mNumVertices, sizeof(unsigned int), 1, file);
            }
            else
            {
                fwrite(&zero, sizeof(unsigned int), 1, file);
            }

            if (mesh->mBitangents != NULL)
            {
                fwrite(&mesh->mNumVertices, sizeof(unsigned int), 1, file);
            }
            else
            {
                fwrite(&zero, sizeof(unsigned int), 1, file);
            }

            for (unsigned int y = 0; y < AI_MAX_NUMBER_OF_TEXTURECOORDS; y++)
            {
                unsigned int count = mesh->mNumVertices * mesh->mNumUVComponents[y];
                fwrite(&count, sizeof(unsigned int), 1, file);
            }

            if (mesh->mFaces != NULL)
            {
                fwrite(&mesh->mNumFaces, sizeof(unsigned int), 1, file);
            }
            else
            {
                fwrite(&zero, sizeof(unsigned int), 1, file);
            }

            if (mesh->mBones != NULL)
            {
                fwrite(&mesh->mNumBones, sizeof(unsigned int), 1, file);
            }
            else
            {
                fwrite(&zero, sizeof(unsigned int), 1, file);
            }

            if (mesh->mVertices != NULL && mesh->mNumVertices > 0)
            {
                fwrite(&mesh->mVertices[0].x, sizeof(ai_real), mesh->mNumVertices * 3, file);
            }
            if (mesh->mNormals != NULL && mesh->mNumVertices > 0)
            {
                fwrite(&mesh->mNormals[0].x, sizeof(ai_real), mesh->mNumVertices * 3, file);
            }
            if (mesh->mTangents != NULL && mesh->mNumVertices > 0)
            {
                fwrite(&mesh->mTangents[0].x, sizeof(ai_real), mesh->mNumVertices * 3, file);
            }
            if (mesh->mBitangents != NULL && mesh->mNumVertices > 0)
            {
                fwrite(&mesh->mBitangents[0].x, sizeof(ai_real), mesh->mNumVertices * 3, file);
            }
            for (unsigned int y = 0; y < AI_MAX_NUMBER_OF_TEXTURECOORDS; y++)
            {
                unsigned int count = mesh->mNumVertices * mesh->mNumUVComponents[y];
                if (count > 0)
                {
                    fwrite(&mesh->mTextureCoords[y]->x, sizeof(float), count, file);
                }
            }
            if (mesh->mFaces != NULL && mesh->mNumFaces > 0)
            {
                for (unsigned int y = 0; y < mesh->mNumFaces; y++)
                {
                    Face *face = &mesh->mFaces[y];
                    fwrite(face->mIndices, sizeof(float), 3, file);
                }
            }
            if (mesh->mBones != NULL && mesh->mNumBones > 0)
            {
                unsigned int *bone_indices = calloc(mesh->mNumVertices * 4, sizeof(unsigned int));
                float *bone_weights = calloc(mesh->mNumVertices * 4, sizeof(float));

                for (unsigned int y = 0; y < mesh->mNumBones; y++)
                {
                    Bone *bone = mesh->mBones[y];
                    for (unsigned int u = 0; u < bone->mNumWeights; u++)
                    {
                        Weight *weight = &bone->mWeights[u];
                        unsigned int bone_index = GPOINTER_TO_UINT(
                            g_hash_table_lookup(bones, bone->mName.data));
                        for (unsigned int v = 0; v < 4; v++)
                        {
                            if (bone_indices[weight->mVertexId * 4 + v] != 0)
                            {
                                bone_indices[weight->mVertexId * 4 + v] = bone_index;
                                bone_weights[weight->mVertexId * 4 + v] = weight->mWeight;
                                break;
                            }
                        }
                    }
                }
                fwrite(bone_indices, sizeof(unsigned int), mesh->mNumBones * 4, file);
                fwrite(bone_weights, sizeof(float), mesh->mNumBones * 4, file);
                free(bone_indices);
                free(bone_weights);
            }

            fclose(file);
        }
        for (unsigned int i = 0; i < scene->mNumTextures; i++)
        {
            Texture *texture = scene->mTextures[i];
            if (texture->mHeight == 0)
            {
                sprintf(path, "%s/tex", res_path);
                mkdir(path);
                sprintf(path, "%s/tex%s", res_path, strrchr(texture->mFilename.data, '/'));
                FILE *file = fopen(path, "wb");
                fwrite(texture->pcData, 1, texture->mWidth, file);
                fclose(file);
            }
        }
        aiReleaseImport(scene);
    }
}

GLuint elf_load_shader(const GLchar *path, GLenum type)
{
    FILE *file = fopen(path, "r");
    GLchar *src_code = malloc(sizeof(GLchar) * 1024);
    for (size_t i = 0; i < 1024; i++)
    {
        char c = (char)fgetc(file);
        if (c != EOF)
        {
            src_code[i] = c;
        }
        else
        {
            src_code[i] = '\0';
            break;
        }
    }
    fclose(file);
    GLuint vertex_shader_id = glCreateShader(type);
    glShaderSource(vertex_shader_id, 1, &src_code, NULL);
    glCompileShader(vertex_shader_id);
    GLint result = GL_FALSE;
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

void elf_import_shader(
    const char *res_path,
    const char *name,
    const char *vertex_path,
    const char *fragment_path)
{
    GLuint program_id = glCreateProgram();

    if (vertex_path != NULL)
    {
        GLuint vert = elf_load_shader(vertex_path, GL_VERTEX_SHADER);
        glAttachShader(program_id, vert);
    }
    if (fragment_path != NULL)
    {
        GLuint frag = elf_load_shader(fragment_path, GL_FRAGMENT_SHADER);
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

    char path[2048];
    sprintf(path, "%s/shd", res_path);
    mkdir(path);
    sprintf(path, "%s/shd/%s", res_path, name);
    FILE *file = fopen(path, "wb");
    fwrite(&format, 1, size, file);
    fwrite(data, 1, size, file);
    fclose(file);
}
