#include <elf_shader.h>

ElfShader elf_shader_create(const char *vertex_code, const char *fragment_code)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_code, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_code, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    ElfShader shader = {
        .id = shaderProgram,
        .vao_id = VAO};

    return shader;
}

void elf_shader_add_vec3_data(ElfShader *shader, float *vectors, unsigned int num_vectors, unsigned int index)
{
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(shader->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vectors * 3, vectors, GL_STATIC_DRAW);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(index);
    shader->vbo_id = VBO;
}

void elf_shader_use(const ElfShader *shader)
{
    glUseProgram(shader->id);
    glBindVertexArray(shader->vao_id);
}

void elf_shader_free(const ElfShader *shader)
{
    glDeleteVertexArrays(1, &shader->vao_id);
    glDeleteBuffers(1, &shader->vbo_id);
    glDeleteProgram(shader->id);
}
