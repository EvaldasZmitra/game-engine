// #include <GL/glew.h>
// #include <stdio.h>
// #include <elf_loader.h>
// #include <SDL.h>
// #include <assimp/cimport.h>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
// #include <assimp/material.h>
// #include <assimp/anim.h>
// #include <glib.h>

// #define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
// #define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
// #define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// typedef struct aiMesh mesh;
// typedef struct aiScene scene;
// typedef struct aiAnimation animation;
// typedef struct aiMaterial material;
// typedef struct aiMesh mesh;
// typedef struct aiSkeleton skeleton;
// typedef struct aiTexture texture;
// typedef struct aiNodeAnim node_anim;
// typedef struct aiMeshAnim mesh_anim;
// typedef struct aiMeshMorphAnim mesh_morph_anim;
// typedef struct aiVectorKey vector_key;
// typedef struct aiQuatKey quat_key;
// typedef struct aiMeshMorphKey mesh_morph_key;
// typedef struct aiBone bone;
// typedef struct aiSkeletonBone skeletal_bone;
// typedef struct aiVertexWeight weight;
// typedef struct aiMaterialProperty property;
// typedef struct aiFace face;

// GLuint elf_load_shader(const GLchar *src_code, GLenum type)
// {
//     GLuint vertex_shader_id = glCreateShader(type);
//     glShaderSource(vertex_shader_id, 1, &src_code, NULL);
//     glCompileShader(vertex_shader_id);

//     GLuint result = GL_FALSE;
//     glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
//     if (!result)
//     {
//         GLint error_length;
//         glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &error_length);
//         GLchar *error_msg = malloc(sizeof(GLchar) * error_length);
//         glGetShaderInfoLog(vertex_shader_id, error_length, NULL, error_msg);
//         fprintf(stderr, "%s\n", error_msg);
//         return -1;
//     }
//     return vertex_shader_id;
// }

// GLuint elf_load_comp(const GLchar *src_code)
// {
//     return elf_load_shader(src_code, GL_COMPUTE_SHADER);
// }

// GLuint elf_load_vert(const GLchar *src_code)
// {
//     return elf_load_shader(src_code, GL_VERTEX_SHADER);
// }

// GLuint elf_load_tess_ctrl(const GLchar *src_code)
// {
//     return elf_load_shader(src_code, GL_TESS_CONTROL_SHADER);
// }

// GLuint elf_load_tess_eval(const GLchar *src_code)
// {
//     return elf_load_shader(src_code, GL_TESS_EVALUATION_SHADER);
// }

// GLuint elf_load_geom(const GLchar *src_code)
// {
//     return elf_load_shader(src_code, GL_GEOMETRY_SHADER);
// }

// GLuint elf_load_frag(const GLchar *src_code)
// {
//     return elf_load_shader(src_code, GL_FRAGMENT_SHADER);
// }

// GLuint elf_load_program(
//     const GLuint comp,
//     const GLuint vert,
//     const GLuint tess_ctrl,
//     const GLuint tess_eval,
//     const GLuint geom,
//     const GLuint frag)
// {
//     GLuint program_id = glCreateProgram();
//     if (comp != NULL)
//     {
//         glAttachShader(program_id, comp);
//     }
//     if (vert != NULL)
//     {
//         glAttachShader(program_id, vert);
//     }
//     if (tess_ctrl != NULL)
//     {
//         glAttachShader(program_id, tess_ctrl);
//     }
//     if (tess_eval != NULL)
//     {
//         glAttachShader(program_id, tess_eval);
//     }
//     if (geom != NULL)
//     {
//         glAttachShader(program_id, geom);
//     }
//     if (frag != NULL)
//     {
//         glAttachShader(program_id, frag);
//     }
//     glLinkProgram(program_id);
//     GLint result = GL_FALSE;
//     glGetProgramiv(program_id, GL_LINK_STATUS, &result);
//     if (!result)
//     {
//         GLint error_length;
//         glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &error_length);
//         GLchar *error_msg = malloc(sizeof(GLchar) * error_length);
//         glGetProgramInfoLog(program_id, error_length, NULL, error_msg);
//         fprintf("%s\n", error_msg);
//         return -1;
//     }
//     return program_id;
// }

// GLuint elf_load_texture(const char *path)
// {
//     unsigned char header[124];
//     FILE *fp;
//     fp = fopen(path, "rb");
//     if (fp == NULL)
//         return 0;
//     char filecode[4];
//     fread(filecode, 1, 4, fp);
//     if (strncmp(filecode, "DDS ", 4) != 0)
//     {
//         fclose(fp);
//         return 0;
//     }
//     fread(&header, 124, 1, fp);
//     unsigned int height = *(unsigned int *)&(header[8]);
//     unsigned int width = *(unsigned int *)&(header[12]);
//     unsigned int linearSize = *(unsigned int *)&(header[16]);
//     unsigned int mipMapCount = *(unsigned int *)&(header[24]);
//     unsigned int fourCC = *(unsigned int *)&(header[80]);
//     unsigned char *buffer;
//     unsigned int bufsize;
//     bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
//     buffer = (unsigned char *)malloc(bufsize * sizeof(unsigned char));
//     fread(buffer, 1, bufsize, fp);
//     fclose(fp);
//     unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
//     unsigned int format;
//     switch (fourCC)
//     {
//     case FOURCC_DXT1:
//         format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//         break;
//     case FOURCC_DXT3:
//         format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//         break;
//     case FOURCC_DXT5:
//         format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//         break;
//     default:
//         free(buffer);
//         return 0;
//     }
//     GLuint textureID;
//     glGenTextures(1, &textureID);
//     glBindTexture(GL_TEXTURE_2D, textureID);
//     unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
//     unsigned int offset = 0;
//     for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
//     {
//         unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
//         glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
//         offset += size;
//         width /= 2;
//         height /= 2;
//         if (width < 1)
//             width = 1;
//         if (height < 1)
//             height = 1;
//     }
//     free(buffer);
//     return textureID;
// }

// GLuint elf_load_vertex_float_buffer(
//     GLuint index,
//     GLsizeiptr data_len,
//     GLint el_len,
//     const GLfloat *data,
//     GLenum usage)
// {
//     GLuint buffer_id;
//     GLsizei data_size = el_len * sizeof(float);
//     glGenBuffers(1, buffer_id);
//     glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
//     glBufferData(GL_ARRAY_BUFFER, data_size * data_len, data, usage);
//     glVertexAttribPointer(index, el_len, GL_FLOAT, GL_FALSE, data_size, NULL);
//     glEnableVertexAttribArray(index);
//     return buffer_id;
// }

// void elf_load_element_array_buffer(
//     const GLuint *indices,
//     GLuint len,
//     GLenum usage)
// {
//     GLuint buffer_id;
//     GLsizeiptr buffer_size = sizeof(GLuint) * len;
//     glGenBuffers(1, buffer_id);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, indices, usage);
//     return buffer_id;
// }

// GLuint elf_load_vec2_float_buffer(
//     GLuint index,
//     const GLfloat *data,
//     GLsizeiptr data_len,
//     GLenum usage)
// {
//     return elf_load_vertex_float_buffer(index, data_len, 2, data, usage);
// }

// GLuint elf_load_vec3_float_buffer(
//     GLuint index,
//     const GLfloat *data,
//     GLsizeiptr data_len,
//     GLenum usage)
// {
//     return elf_load_vertex_float_buffer(index, data_len, 3, data, usage);
// }

// elf_model elf_load_model(const char *path, unsigned int flags)
// {
//     const scene *scene = aiImportFile(path, flags);
//     elf_model model;
//     int current_bone_id = 0;
//     GHashTable *bones = g_hash_table_new(g_str_hash, g_str_equal);
//     if (scene->mNumMeshes > 0)
//     {
//         model.meshes = malloc(scene->mNumMeshes * sizeof(elf_mesh));
//         for (unsigned int i = 0; i < scene->mNumMeshes; i++)
//         {
//             mesh *mesh = scene->mMeshes[i];
//             elf_mesh *elf_mesh = &model.meshes[i];
//             elf_mesh->name = malloc(mesh->mName.length * sizeof(char));
//             memcpy(elf_mesh->name, mesh->mName.data, mesh->mName.length * sizeof(char));
//             elf_mesh->vertices = malloc(mesh->mNumVertices * 3 * sizeof(GLfloat));
//             elf_mesh->indices = malloc(mesh->mNumFaces * 3 * sizeof(GLuint));
//             if (mesh->mBones != NULL)
//             {
//                 elf_mesh->weights = malloc(mesh->mNumVertices * 4 * sizeof(GLint));
//                 elf_mesh->bone_ids = malloc(mesh->mNumVertices * 4 * sizeof(GLint));
//             }
//             if (mesh->mNormals != NULL)
//             {
//                 elf_mesh->normals = malloc(mesh->mNumVertices * 3 * sizeof(GLfloat));
//             }
//             if (mesh->mTangents != NULL)
//             {
//                 elf_mesh->tangents = malloc(mesh->mNumVertices * 3 * sizeof(GLfloat));
//             }
//             if (mesh->mBitangents != NULL)
//             {
//                 elf_mesh->bit_tangents = malloc(mesh->mNumVertices * 3 * sizeof(GLfloat));
//             }
//             for (unsigned int u = 0; u < AI_MAX_NUMBER_OF_COLOR_SETS; u++)
//             {
//                 if (mesh->mColors[u] != NULL)
//                 {
//                     elf_mesh->colors[u] = malloc(4 * mesh->mNumVertices * sizeof(GLfloat));
//                 }
//             }
//             for (unsigned int u = 0; u < AI_MAX_NUMBER_OF_TEXTURECOORDS; u++)
//             {
//                 if (mesh->mTextureCoords[u] != NULL)
//                 {
//                     elf_mesh->texture_coords[u] = malloc(mesh->mNumUVComponents[u] * mesh->mNumVertices * sizeof(GLfloat));
//                 }
//             }
//             for (unsigned int y = 0; y < mesh->mNumVertices; y++)
//             {
//                 elf_mesh->vertices[y * 3] = mesh->mVertices[y].x;
//                 elf_mesh->vertices[y * 3 + 1] = mesh->mVertices[y].y;
//                 elf_mesh->vertices[y * 3 + 2] = mesh->mVertices[y].z;

//                 if (mesh->mNormals != NULL)
//                 {
//                     elf_mesh->normals[y * 3] = mesh->mNormals[y].x;
//                     elf_mesh->normals[y * 3 + 1] = mesh->mNormals[y].y;
//                     elf_mesh->normals[y * 3 + 2] = mesh->mNormals[y].z;
//                 }

//                 if (mesh->mTangents != NULL)
//                 {
//                     elf_mesh->tangents[y * 3] = mesh->mTangents[y].x;
//                     elf_mesh->tangents[y * 3 + 1] = mesh->mTangents[y].y;
//                     elf_mesh->tangents[y * 3 + 2] = mesh->mTangents[y].z;
//                 }

//                 if (mesh->mBitangents != NULL)
//                 {
//                     elf_mesh->bit_tangents[y * 3] = mesh->mBitangents[y].x;
//                     elf_mesh->bit_tangents[y * 3 + 1] = mesh->mBitangents[y].y;
//                     elf_mesh->bit_tangents[y * 3 + 2] = mesh->mBitangents[y].z;
//                 }

//                 if (mesh->mBones != NULL)
//                 {
//                     elf_mesh->weights[y * 3] = 0;
//                     elf_mesh->weights[y * 3 + 1] = 0;
//                     elf_mesh->weights[y * 3 + 2] = 0;

//                     elf_mesh->bone_ids[y * 3] = -1;
//                     elf_mesh->bone_ids[y * 3 + 1] = -1;
//                     elf_mesh->bone_ids[y * 3 + 2] = -1;
//                 }

//                 for (unsigned int u = 0; u < AI_MAX_NUMBER_OF_TEXTURECOORDS; u++)
//                 {
//                     if (mesh->mTextureCoords[u] != NULL)
//                     {
//                         unsigned int num_components = mesh->mNumUVComponents[u];
//                         for (unsigned int v = 0; v < num_components; v++)
//                         {
//                             if (v == 0)
//                             {
//                                 elf_mesh->texture_coords[u][y * num_components] = mesh->mTextureCoords[u][y].x;
//                             }
//                             if (v == 1)
//                             {
//                                 elf_mesh->texture_coords[u][y * num_components + 1] = mesh->mTextureCoords[u][y].y;
//                             }
//                             if (v == 2)
//                             {
//                                 elf_mesh->texture_coords[u][y * num_components + 2] = mesh->mTextureCoords[u][y].z;
//                             }
//                         }
//                     }
//                 }

//                 for (unsigned int u = 0; u < AI_MAX_NUMBER_OF_COLOR_SETS; u++)
//                 {
//                     if (mesh->mColors[u] != NULL)
//                     {
//                         elf_mesh->colors[u][y * 4] = mesh->mColors[u][y].r;
//                         elf_mesh->colors[u][y * 4 + 1] = mesh->mColors[u][y].g;
//                         elf_mesh->colors[u][y * 4 + 2] = mesh->mColors[u][y].b;
//                         elf_mesh->colors[u][y * 4 + 3] = mesh->mColors[u][y].a;
//                     }
//                 }
//             }
//             for (unsigned int y = 0; y < mesh->mNumBones; y++)
//             {
//                 bone *bone = &mesh->mBones[y];
//                 if (g_hash_table_contains(bones, bone->mName.data))
//                 {
//                     GLint bone_id = GPOINTER_TO_INT(g_hash_table_lookup(bones, bone->mName.data));
//                     for (unsigned int u = 0; u < bone->mNumWeights; u++)
//                     {
//                         weight *weight = &bone->mWeights[u];
//                         for (unsigned v = 0; v < 4; v++)
//                         {
//                             unsigned int ind = weight->mVertexId * 4 + v;
//                             if (elf_mesh->indices[ind] == -1)
//                             {
//                                 elf_mesh->weights[ind] = weight->mVertexId;
//                                 elf_mesh->bone_ids[ind] = bone_id;
//                             }
//                         }
//                     }
//                 }
//             }
//             for (unsigned int y = 0; y < mesh->mNumFaces; y++)
//             {
//                 face *face = &mesh->mFaces[y];
//                 for (unsigned int u = 0; u < face->mNumIndices; u++)
//                 {
//                     elf_mesh->indices[y * 3 + y] = face->mIndices[u];
//                 }
//             }
//         }
//     }
//     if (scene->mNumTextures > 0)
//     {
//         model.texture = malloc(sizeof(scene->mNumTextures * sizeof(elf_texture)));
//         for (unsigned int i = 0; i < scene->mNumTextures; i++)
//         {
//             texture *texture = scene->mTextures[i];
//             elf_texture *elf_texture = &model.texture[i];

//             texture->achFormatHint;
//             texture->mFilename;
//             texture->mHeight;
//             texture->mWidth;
//             texture->pcData;
//         }
//     }
//     for (unsigned int i = 0; i < scene->mNumTextures; i++)
//     {
//         texture *texture = &scene->mTextures[i];
//         texture->achFormatHint;
//         texture->mFilename;
//         texture->mHeight;
//         texture->mWidth;
//         texture->pcData;
//     }
//     for (unsigned int i = 0; i < scene->mNumAnimations; i++)
//     {
//         animation *animation = &scene->mAnimations[i];
//         animation->mTicksPerSecond;
//         animation->mDuration;
//         animation->mName;
//         for (unsigned int y = 0; y < animation->mNumChannels; y++)
//         {
//             node_anim *channel = animation->mChannels[y];
//             channel->mNodeName;
//             channel->mPostState;
//             channel->mPreState;
//             for (unsigned int u; u < channel->mNumPositionKeys; u++)
//             {
//                 vector_key *positionKey = &channel->mPositionKeys[u];
//                 positionKey->mTime;
//                 positionKey->mValue;
//             }
//             for (unsigned int u; u < channel->mNumRotationKeys; u++)
//             {
//                 quat_key *rotation_key = &channel->mRotationKeys[u];
//                 rotation_key->mTime;
//                 rotation_key->mValue;
//             }
//             for (unsigned int u; u < channel->mNumScalingKeys; u++)
//             {
//                 vector_key *scalingKey = &channel->mScalingKeys[u];
//                 scalingKey->mTime;
//                 scalingKey->mValue;
//             }
//         }
//         for (unsigned int y = 0; y < animation->mNumMeshChannels; y++)
//         {
//             mesh_anim *mesh_channel = &animation->mMeshChannels[y];
//             mesh_channel->mName;
//             for (unsigned int u; u < mesh_channel->mNumKeys; u++)
//             {
//                 mesh_morph_key *key = &mesh_channel->mKeys[u];
//                 key->mTime;
//                 for (unsigned int v = 0; v < key->mNumValuesAndWeights; v++)
//                 {
//                     key->mValues[v];
//                     key->mWeights[v];
//                 }
//             }
//         }
//         for (unsigned int y = 0; y < animation->mNumMorphMeshChannels; y++)
//         {
//             mesh_morph_anim *morph_mesh_channel = &animation->mMorphMeshChannels[y];
//             morph_mesh_channel->mName;
//             for (unsigned int u; u < morph_mesh_channel->mNumKeys; u++)
//             {
//                 mesh_morph_key *key = &morph_mesh_channel->mKeys[u];
//                 key->mTime;
//                 for (unsigned int v = 0; v < key->mNumValuesAndWeights; v++)
//                 {
//                     key->mValues[v];
//                     key->mWeights[v];
//                 }
//             }
//         }
//     }
//     return model;
// }

// // for (unsigned int i = 0; i < scene->mNumSkeletons; i++)
// // {
// //     skeleton *skeleton = &scene->mSkeletons[i];
// //     elf_skeleton *elf_skeleton = &model.skeletons[i];
// //     elf_skeleton->name = malloc(skeleton->mName.length * sizeof(GLchar));
// //     memcpy(elf_skeleton->name, skeleton->mName.data, skeleton->mName.length * sizeof(GLchar));
// //     elf_skeleton->bones = malloc(skeleton->mNumBones * sizeof(elf_bone));
// //     for (unsigned int y = 0; y < skeleton->mNumBones; i++, current_bone_id++)
// //     {
// //         skeletal_bone *bone = &skeleton->mBones[y];
// //         elf_bone *elf_bone = &elf_skeleton->bones[y];
// //         size_t bone_name_size = bone->mNode->mName.length * sizeof(GLchar);
// //         elf_bone->name = malloc(bone_name_size);
// //         memcpy(elf_bone->name, bone->mNode->mName.data, bone_name_size);

// //         elf_bone->offset[0][0] = bone->mOffsetMatrix.a1;
// //         elf_bone->offset[0][1] = bone->mOffsetMatrix.a2;
// //         elf_bone->offset[0][2] = bone->mOffsetMatrix.a3;
// //         elf_bone->offset[0][3] = bone->mOffsetMatrix.a4;

// //         elf_bone->offset[1][0] = bone->mOffsetMatrix.b1;
// //         elf_bone->offset[1][1] = bone->mOffsetMatrix.b2;
// //         elf_bone->offset[1][2] = bone->mOffsetMatrix.b3;
// //         elf_bone->offset[1][3] = bone->mOffsetMatrix.b4;

// //         elf_bone->offset[2][0] = bone->mOffsetMatrix.c1;
// //         elf_bone->offset[2][1] = bone->mOffsetMatrix.c2;
// //         elf_bone->offset[2][2] = bone->mOffsetMatrix.c3;
// //         elf_bone->offset[2][3] = bone->mOffsetMatrix.c4;

// //         elf_bone->offset[3][0] = bone->mOffsetMatrix.d1;
// //         elf_bone->offset[3][1] = bone->mOffsetMatrix.d2;
// //         elf_bone->offset[3][2] = bone->mOffsetMatrix.d3;
// //         elf_bone->offset[3][3] = bone->mOffsetMatrix.d4;

// //         elf_bone->parent_id = bone->mParent;

// //         g_hash_table_insert(
// //             bones,
// //             bone->mNode->mName.data,
// //             GINT_TO_POINTER(current_bone_id));
// //     }
// // }