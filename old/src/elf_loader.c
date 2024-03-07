#include <SDL_image.h>
#include <GL/glew.h>
#include <elf_model.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

typedef struct aiMesh Mesh;
typedef struct aiScene Scene;

unsigned int elf_load_texture(const char *path)
{
    unsigned int textureID;
    SDL_Surface *img = IMG_Load(path);
    int mode = GL_RGB;
    if (img->format->BytesPerPixel == 4)
    {
        mode = GL_RGBA;
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, img->w, img->h, 0, mode, GL_UNSIGNED_BYTE, img->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void elf_model_load_vertices(Mesh *mesh, unsigned int *buffers)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 12, mesh->mVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 12, NULL);
    glEnableVertexAttribArray(VERTICES);
}

void elf_model_load_texture_coords(Mesh *mesh, unsigned int *buffers)
{
    for (unsigned int channel = 0; channel < 8; channel++)
    {
        unsigned int num_uv_components = mesh->mNumUVComponents[channel];
        if (num_uv_components == 2)
        {
            unsigned int buffer_size = mesh->mNumVertices * 8;
            float *buffer = malloc(buffer_size);
            for (unsigned i = 0; i < mesh->mNumVertices; i++)
            {
                buffer[i * 2] = mesh->mTextureCoords[channel][i].x;
                buffer[i * 2 + 1] = mesh->mTextureCoords[channel][i].y;
            }
            unsigned int texture_coords = TEXTURE_COORDS_0 + channel;
            glBindBuffer(GL_ARRAY_BUFFER, buffers[texture_coords]);
            glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STATIC_DRAW);
            glVertexAttribPointer(texture_coords, 2, GL_FLOAT, GL_FALSE, 8, NULL);
            glEnableVertexAttribArray(texture_coords);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            free(buffer);
        }
    }
}

void elf_model_load_normals(Mesh *mesh, unsigned int *buffers)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 12, mesh->mNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, 12, NULL);
    glEnableVertexAttribArray(NORMALS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void elf_model_load_tangents(Mesh *mesh, unsigned int *buffers)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[TANGENTS]);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 12, mesh->mTangents, GL_STATIC_DRAW);
    glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, 12, NULL);
    glEnableVertexAttribArray(TANGENTS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void elf_model_load_bit_tangents(Mesh *mesh, unsigned int *buffers)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[BIT_TANGENTS]);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 12, mesh->mBitangents, GL_STATIC_DRAW);
    glVertexAttribPointer(BIT_TANGENTS, 3, GL_FLOAT, GL_FALSE, 12, NULL);
    glEnableVertexAttribArray(BIT_TANGENTS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void elf_model_load_bones(Mesh *mesh, unsigned int *buffers)
{
    GLsizei component_size = 16;
    unsigned int buffer_size = mesh->mNumVertices * component_size;
    float *weights = malloc(buffer_size);
    int *ids = malloc(buffer_size);

    for (unsigned int i = 0; i < mesh->mNumVertices * 4; i++)
    {
        ids[i] = -1;
    }

    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        struct aiBone *bone = mesh->mBones[i];
        for (unsigned int y = 0; y < bone->mNumWeights; y++)
        {
            struct aiVertexWeight weight = bone->mWeights[y];
            unsigned int index = weight.mVertexId * 4;
            for (unsigned int z = index; z < index + 4; z++)
            {
                if (ids[z] != -1)
                {
                    ids[z] = weight.mVertexId;
                    weights[z] = weight.mWeight;
                }
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[BONE_WEIGHTS]);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, weights, GL_STATIC_DRAW);
    glVertexAttribPointer(BONE_WEIGHTS, 4, GL_FLOAT, GL_FALSE, 16, NULL);
    glEnableVertexAttribArray(BONE_WEIGHTS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[BONE_IDS]);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, ids, GL_STATIC_DRAW);
    glVertexAttribPointer(BONE_IDS, 4, GL_INT, GL_FALSE, 16, NULL);
    glEnableVertexAttribArray(BONE_IDS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(weights);
    free(ids);
}

void elf_model_load_indices(Mesh *mesh, unsigned int *buffers)
{
    unsigned int buffer_size = mesh->mNumFaces * 4 * 3;
    unsigned int *indices = malloc(buffer_size);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        struct aiFace face = mesh->mFaces[i];
        int index = i * 3;
        indices[index] = face.mIndices[0];
        indices[index + 1] = face.mIndices[1];
        indices[index + 2] = face.mIndices[2];
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, indices, GL_STATIC_DRAW);
    free(indices);
}

ElfModels elf_model_load(const char *path)
{
    unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
    const Scene *scene = aiImportFile(path, flags);
    ElfModels result;
    result.models = malloc(sizeof(ElfModel) * scene->mNumMeshes);
    result.num_models = scene->mNumMeshes;
    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        ElfModel *model = &result.models[i];
        Mesh *mesh = scene->mMeshes[i];
        glGenVertexArrays(1, &model->vao);
        glGenBuffers(NUM_BUFFERS, model->buffers);
        glBindVertexArray(model->vao);
        elf_model_load_vertices(mesh, model->buffers);
        elf_model_load_texture_coords(mesh, model->buffers);
        elf_model_load_normals(mesh, model->buffers);
        elf_model_load_tangents(mesh, model->buffers);
        elf_model_load_bit_tangents(mesh, model->buffers);
        elf_model_load_bones(mesh, model->buffers);
        elf_model_load_indices(mesh, model->buffers);
        glBindVertexArray(0);
        model->num_indices = mesh->mNumFaces * 3;
    }
    aiReleaseImport(scene);
    return result;
}
