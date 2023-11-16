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
        fprintf(stderr, "Error reading the file: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }
}
