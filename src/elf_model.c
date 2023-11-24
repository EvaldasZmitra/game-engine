#include <stdlib.h>
#include <memory.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <stdio.h>

#include <elf_model.h>

typedef struct aiMesh Mesh;
typedef struct aiScene Scene;
unsigned int *indices;
Mesh *meshas;

void elf_model_load_vertices(Mesh *mesh, unsigned int *buffers)
{
    meshas = mesh;
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 12, &mesh->mVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 12, NULL);
    glEnableVertexAttribArray(VERTICES);
}

void elf_model_load_colors(Mesh *mesh, unsigned int *buffers)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[COLORS]);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 16, mesh->mColors, GL_STATIC_DRAW);
    glVertexAttribPointer(COLORS, 3, GL_FLOAT, GL_FALSE, 16, NULL);
    glEnableVertexAttribArray(COLORS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void elf_model_load_texture_coords(Mesh *mesh, unsigned int *buffers)
{
    GLsizei component_size = 8;
    unsigned int buffer_size = mesh->mNumVertices * component_size;

    float *buffer = malloc(buffer_size);
    for (unsigned i = 0; i < mesh->mNumVertices; i += 2)
    {
        buffer[i] = mesh->mTextureCoords[0][i].x;
        buffer[i + 1] = mesh->mTextureCoords[0][i].y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXTURE_COORDS]);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(TEXTURE_COORDS, 2, GL_FLOAT, GL_FALSE, 8, NULL);
    glEnableVertexAttribArray(TEXTURE_COORDS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(buffer);
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
    indices = malloc(buffer_size);
    for (int i = 0; i < mesh->mNumFaces; i++)
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

ElfModel elf_model_load(const char *path)
{
    unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;
    const Scene *scene = aiImportFile(path, flags);
    Mesh *mesh = scene->mMeshes[0];

    ElfModel model;

    glGenVertexArrays(1, &model.vao);
    glGenBuffers(NUM_BUFFERS, model.buffers);
    glBindVertexArray(model.vao);
    elf_model_load_vertices(mesh, model.buffers);
    elf_model_load_colors(mesh, model.buffers);
    elf_model_load_texture_coords(mesh, model.buffers);
    elf_model_load_normals(mesh, model.buffers);
    elf_model_load_tangents(mesh, model.buffers);
    elf_model_load_bit_tangents(mesh, model.buffers);
    elf_model_load_bones(mesh, model.buffers);
    elf_model_load_indices(mesh, model.buffers);
    glBindVertexArray(0);

    model.num_indices = mesh->mNumFaces * 3;

    aiReleaseImport(scene);
    return model;
}

void elf_model_free(ElfModel *model)
{
    glDeleteBuffers(NUM_BUFFERS, model->buffers);
    glDeleteVertexArrays(1, &model->vao);
}

void elf_model_draw(ElfModel *model)
{
    glBindVertexArray(model->vao);
    glDrawElements(GL_TRIANGLES, model->num_indices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void elf_model_draw_instanced(ElfModel *model, int num_instances)
{
    glBindVertexArray(model->vao);
    glDrawElementsInstanced(GL_TRIANGLES, model->num_indices, GL_UNSIGNED_INT, NULL, num_instances);
    glBindVertexArray(0);
}
