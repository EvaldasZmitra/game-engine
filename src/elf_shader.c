#include <elf_shader.h>

float vertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f};

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

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    ElfShader shader = {
        .id = shaderProgram,
        .vao_id = VAO};

    return shader;
}

void elf_shader_use(const ElfShader *shader)
{
    glUseProgram(shader->id);
}

void elf_shader_free(const ElfShader *shader)
{
    glDeleteVertexArrays(1, &shader->vao_id);
    glDeleteBuffers(1, &shader->vbo_id);
    glDeleteProgram(shader->id);
}
