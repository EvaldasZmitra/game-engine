#include <elf_io.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct stat st = {0};

char *elf_read(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_contents = (char *)malloc(file_size + 1);
    if (file_contents == NULL)
    {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }
    fread(file_contents, 1, file_size, file);
    file_contents[file_size] = '\0';
    fclose(file);
    return file_contents;
}

void elf_write_buffer(const elf_str *path, void *buffer, unsigned int buffer_size)
{
    elf_str dir = elf_str_get_path(path);
    if (stat(dir.chars, &st) == -1)
    {
        mkdir(dir.chars);
    }
    FILE *file = fopen(path->chars, "wb");
    fwrite(buffer, 1, buffer_size, file);
    fclose(file);
    elf_str_free(&dir);
}

void elf_append_buffer(const elf_str *path, void *buffer, unsigned int buffer_size)
{
    elf_str dir = elf_str_get_path(path);
    if (stat(dir.chars, &st) == -1)
    {
        mkdir(dir.chars);
    }
    FILE *file = fopen(path->chars, "wb+");
    fwrite(buffer, 1, buffer_size, file);
    fclose(file);
    elf_str_free(&dir);
}
