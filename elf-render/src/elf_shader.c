#include <elf_shader.h>

unsigned int elf_shader_create(const char *vertex_code, const char *fragment_code)
{
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_code, NULL);
    glCompileShader(vertex_shader_id);

    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_code, NULL);
    glCompileShader(fragment_shader_id);

    unsigned int shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader_id);
    glAttachShader(shader_id, fragment_shader_id);
    glLinkProgram(shader_id);
    glUseProgram(shader_id);

    return shader_id;
}

void elf_shader_use(unsigned int shader_id)
{
    glUseProgram(shader_id);
}

void elf_shader_destroy(unsigned int shader_id)
{
    glDeleteProgram(shader_id);
}

void elf_shader_set_matrix4x4(unsigned int shader_id, const char *name, float *data)
{
    int uniform_id = glGetUniformLocation(shader_id, name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, data);
}

void elf_shader_set_vec4(unsigned int shader_id, const char *name, float *data)
{
    int uniform_id = glGetUniformLocation(shader_id, name);
    glUniform4fv(uniform_id, 1, data);
}

void elf_shader_set_vec3(unsigned int shader_id, const char *name, float *data)
{
    int uniform_id = glGetUniformLocation(shader_id, name);
    glUniform3fv(uniform_id, 1, data);
}

void elf_shader_set_vec2(unsigned int shader_id, const char *name, float *data)
{
    int uniform_id = glGetUniformLocation(shader_id, name);
    glUniform2fv(uniform_id, 1, data);
}
