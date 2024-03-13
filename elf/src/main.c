#include <stdio.h>
#include <elf_window.h>
#include <elf_load.h>
#include <sys/stat.h>

#undef main

GLuint elf_load_shader2(const GLchar *path, GLenum type)
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

GLuint elf_import_shader2(
    const char *res_path,
    const char *name,
    const char *vertex_path,
    const char *fragment_path)
{
    GLuint program_id = glCreateProgram();

    if (vertex_path != NULL)
    {
        GLuint vert = elf_load_shader2(vertex_path, GL_VERTEX_SHADER);
        glAttachShader(program_id, vert);
    }
    if (fragment_path != NULL)
    {
        GLuint frag = elf_load_shader2(fragment_path, GL_FRAGMENT_SHADER);
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
    fwrite(&format, sizeof(GLenum), 1, file);
    fwrite(data, 1, size, file);
    fclose(file);

    GLuint program_id1 = glCreateProgram();
    FILE *file2 = fopen(path, "rb");
    fseek(file2, 0, SEEK_END);
    GLsizeiptr size2 = ftell(file2) - sizeof(GLenum);
    fseek(file2, 0, SEEK_SET);

    char *buffer = malloc(size2);
    fread(&format, sizeof(GLenum), 1, file2);
    fread(buffer, size2, 1, file2);
    glProgramBinary(program_id1, format, buffer, size2);
    fclose(file2);
    return program_id1;
}

GLuint elf_load_shader1(const char *path)
{
    GLenum format;
    GLuint program_id1 = glCreateProgram();
    FILE *file2 = fopen(path, "rb");
    fseek(file2, 0, SEEK_END);
    GLsizeiptr size2 = ftell(file2) - sizeof(GLenum);
    fseek(file2, 0, SEEK_SET);

    char *buffer = malloc(size2);
    fread(&format, sizeof(GLenum), 1, file2);
    fread(buffer, size2, 1, file2);
    glProgramBinary(program_id1, format, buffer, size2);
    fclose(file2);
    return program_id1;
}

int main()
{
    SDL_Window *window = elf_window_create(
        "Title",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        800,
        SDL_WINDOW_OPENGL);
    SDL_Event event;
    int running = 1;

    GLuint shader_id2 = elf_import_shader2(
        "C:\\Projects\\game-engine\\elf\\build\\res",
        "default",
        "C:\\Projects\\game-engine\\elf-import\\res\\default.vert",
        "C:\\Projects\\game-engine\\elf-import\\res\\default.frag");
    GLuint shader_id = elf_load_shader1("C:\\Projects\\game-engine\\elf\\build\\res\\shd\\default");
    // GLuint shader_id = elf_import_shader(
    //     "C:\\Projects\\game-engine\\elf-import\\res\\default.vert",
    //     "C:\\Projects\\game-engine\\elf-import\\res\\default.frag");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,
        -1.0f,
        0.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
    };
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.1f, 0.3f, 0.0f);
        glUseProgram(shader_id);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        SDL_GL_SwapWindow(window);
    }
    return 0;
}
