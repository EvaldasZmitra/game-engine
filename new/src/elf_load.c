#include <GL/glew.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cglm/cglm.h>
#include <elf_load.h>
#include <glib.h>

GLuint elf_load_buffer(
    GLsizeiptr size,
    const void *data,
    GLenum target,
    GLenum usage)
{
    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(target, buffer_id);
    glBufferData(target, size, data, usage);
    return buffer_id;
}

GLuint elf_load_file_buffer(const char *path, GLenum target, GLenum usage)
{
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    GLsizeiptr size = ftell(file);
    fseek(file, 0, SEEK_SET);
    const void *buffer = malloc(size);
    fread(buffer, 1, size, file);
    GLuint buffer_id = elf_load_buffer(size, buffer, target, usage);
    free(buffer);
    fclose(file);
    return buffer_id;
}

void elf_load_file_vao_buffer(FILE *file)
{
    char path[1024];
    GLenum target;
    GLenum usage;
    fgets(path, sizeof(path), file);
    fscanf(file, "%d", &target);
    fscanf(file, "%d", &usage);
    GLuint buffer_id = elf_load_file_buffer(path, target, usage);
    glBindBuffer(target, buffer_id);
    if (target == GL_VERTEX_ARRAY)
    {
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        GLuint offset;
        fscanf(file, "%d", &index);
        fscanf(file, "%d", &size);
        fscanf(file, "%d", &type);
        fscanf(file, "%d", &normalized);
        fscanf(file, "%d", &stride);
        fscanf(file, "%d", &offset);
        glVertexAttribPointer(index, size, type, normalized, stride, (GLvoid *)offset);
        glEnableVertexAttribArray(index);
    }
    fgets(path, sizeof(path), file);
}

GLuint elf_load_file_shader(const char *path)
{
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    GLsizeiptr size = ftell(file);
    fseek(file, 0, SEEK_SET);
    GLenum format;
    void *buffer = malloc(size);
    fread(&format, sizeof(GLenum), 1, file);
    fread(buffer, 1, size - sizeof(GLenum), file);
    GLuint program_id = glCreateProgram();
    glProgramBinary(program_id, format, buffer, size);
    free(buffer);
    fclose(file);
}

GLuint elf_load_file_texture(const char *path)
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

elf_bone *elf_load_file_skeleton(const char *path)
{
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    GLsizeiptr size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned int num_bones = size / 68;
    elf_bone *bones = malloc(size);
    for (unsigned int i = 0; i < num_bones; i++)
    {
        elf_bone *bone = &bones[i];
        fread(&bone->parent, sizeof(GLuint), 1, file);
        fread(&bone->offset[0][0], sizeof(GLuint), 16, file);
    }
    fclose(file);
    return bones;
}

elf_animation elf_load_file_animation(const char *path)
{
    FILE *file = fopen(path, "rb");
    elf_animation result;

    fread(&result.num_position_keys, sizeof(GLuint), 1, file);
    fread(&result.num_rotation_keys, sizeof(GLuint), 1, file);
    fread(&result.num_scale_keys, sizeof(GLuint), 1, file);

    result.position_keys = malloc(result.num_position_keys * sizeof(GLfloat));
    result.rotation_keys = malloc(result.num_rotation_keys * sizeof(GLfloat));
    result.scale_keys = malloc(result.num_scale_keys * sizeof(GLfloat));

    result.position_times = malloc(result.num_position_keys * sizeof(GLuint));
    result.rotation_times = malloc(result.num_rotation_keys * sizeof(GLuint));
    result.scale_times = malloc(result.num_scale_keys * sizeof(GLuint));

    fread(result.position_keys, sizeof(GLfloat), result.num_position_keys, file);
    fread(result.rotation_keys, sizeof(GLfloat), result.num_rotation_keys, file);
    fread(result.scale_keys, sizeof(GLfloat), result.num_scale_keys, file);

    fread(result.position_times, sizeof(GLfloat), result.num_position_keys, file);
    fread(result.rotation_times, sizeof(GLfloat), result.num_rotation_keys, file);
    fread(result.scale_times, sizeof(GLfloat), result.num_scale_keys, file);

    fclose(file);
    return result;
}

GLuint elf_load_file_vao(const char *path)
{
    FILE *file = fopen(path, "r");
    unsigned int num_buffers;
    fscanf(file, "%d", &num_buffers);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    for (unsigned int i = 0; i < num_buffers; i++)
    {
        elf_load_file_vao_buffer(file);
    }
    glBindVertexArray(0);
    fclose(file);
}

GLuint elf_load_res_buffer(
    const elf_loader *loader,
    char *name,
    GLenum target,
    GLenum usage)
{
    if (!g_hash_table_contains(loader->cache, name))
    {
        char path[1024];
        snprintf(path, 1024, "%s/%s", loader->res_path, name);
        elf_join_path(path, loader->res_path, name);
        return elf_load_file_buffer(path, target, usage);
    }
    else
    {
        return GPOINTER_TO_UINT(g_hash_table_lookup(loader->cache, name));
    }
}

GLuint elf_load_res_shader(const elf_loader *loader, char *name)
{
    if (!g_hash_table_contains(loader->cache, name))
    {
        char path[1024];
        snprintf(path, 1024, "%s/%s", loader->res_path, name);
        return elf_load_file_shader(path);
    }
    else
    {
        return GPOINTER_TO_UINT(g_hash_table_lookup(loader->cache, name));
    }
}

GLuint elf_load_res_texture(const elf_loader *loader, char *name)
{
    if (!g_hash_table_contains(loader->cache, name))
    {
        char path[1024];
        snprintf(path, 1024, "%s/%s", loader->res_path, name);
        return elf_load_file_texture(path);
    }
    else
    {
        return GPOINTER_TO_UINT(g_hash_table_lookup(loader->cache, name));
    }
}

GLuint elf_load_res_skeleton(const elf_loader *loader, char *name)
{
    if (!g_hash_table_contains(loader->cache, name))
    {
        char path[1024];
        snprintf(path, 1024, "%s/%s", loader->res_path, name);
        return elf_load_file_skeleton(path);
    }
    else
    {
        return GPOINTER_TO_UINT(g_hash_table_lookup(loader->cache, name));
    }
}

elf_animation elf_load_res_animation(const elf_loader *loader, char *name)
{
    if (!g_hash_table_contains(loader->cache, name))
    {
        char path[1024];
        snprintf(path, 1024, "%s/%s", loader->res_path, name);
        return elf_load_file_animation(path);
    }
    else
    {
        return *((elf_animation *)g_hash_table_lookup(loader->cache, name));
    }
}

GLuint elf_load_res_vao(const elf_loader *loader, char *name)
{
    if (!g_hash_table_contains(loader->cache, name))
    {
        char path[1024];
        snprintf(path, 1024, "%s/%s", loader->res_path, name);
        return elf_load_file_vao(path);
    }
    else
    {
        return GPOINTER_TO_UINT(g_hash_table_lookup(loader->cache, name));
    }
}

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

void elf_load_program(
    const char *path,
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