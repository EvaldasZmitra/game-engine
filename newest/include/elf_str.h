#ifndef ELF_STR_H
#define ELF_STR_H

typedef struct elf_str
{
    unsigned int len;
    char *chars;
} elf_str;

elf_str elf_str_init(const char *chars, unsigned int len);
elf_str elf_str_init_null(const char *chars, unsigned int max_len);
void elf_str_free(const elf_str *str);
elf_str elf_str_copy(const elf_str *str);
elf_str elf_str_slice(const elf_str *str, unsigned int start, unsigned int end);
elf_str elf_str_add(const elf_str *str, const elf_str *other);
unsigned int elf_str_r_index(const elf_str *str, char symbol);
unsigned int elf_str_l_index(const elf_str *str, char symbol);
elf_str elf_str_replace_path(const elf_str *path, const elf_str *replace);
elf_str elf_str_get_path(const elf_str *path);

#endif