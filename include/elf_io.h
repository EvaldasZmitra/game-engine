#ifndef ELF_IO_H
#define ELF_IO_H

#include <glib.h>
#include <stdio.h>

gchar *elf_io_read(const char *file_path);
void elf_io_write_append(char *file_path, char *text);

#endif