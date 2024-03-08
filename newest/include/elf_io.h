#ifndef ELF_IO_H
#define ELF_IO_H
#include <elf_str.h>

void elf_write_buffer(const elf_str *path, void *buffer, unsigned int buffer_size);
void elf_append_buffer(const elf_str *path, void *buffer, unsigned int buffer_size);
char *elf_read(const char *path);

#endif