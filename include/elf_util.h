#ifndef ELF_UTIL_H
#define ELF_UTIL_H

#include <glib.h>
#include <stdio.h>

gchar *elf_util_read(const char *file_path);

#endif