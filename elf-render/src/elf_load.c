#include <elf_load.h>
#include <stdio.h>
#include <stdlib.h>

GLuint elf_load_shader(const char *path)
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
    return 0;
}
