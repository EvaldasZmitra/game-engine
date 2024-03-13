
#ifndef ELF_IMPORT_H
#define ELF_IMPORT_H

void elf_import_model(
    const char *model_path,
    const char *res_path,
    unsigned int flags);

void elf_import_shader(
    const char *res_path,
    const char *name,
    const char *vertex_path,
    const char *fragment_path);

#endif
