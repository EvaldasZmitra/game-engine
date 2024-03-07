#include <elf_io.h>

gchar *elf_io_read(const char *file_path)
{
    GError *error = NULL;
    gchar *contents = NULL;
    if (g_file_get_contents(file_path, &contents, NULL, &error))
    {
        return contents;
    }
    else
    {
        g_error_free(error);
        return NULL;
    }
}

void elf_io_write_append(char *file_path, char *text)
{
    FILE *file = fopen(file_path, "a");
    fprintf(file, text);
    fclose(file);
}
