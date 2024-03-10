#ifndef ELF_IO_H
#define ELF_IO_H

void elf_write_buffer(const char *path, void *buffer, unsigned int buffer_size);
void elf_append_buffer(const char *path, void *buffer, unsigned int buffer_size);
char *elf_read(const char *path);

#endif