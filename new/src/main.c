#include <elf_import.h>
#include <elf_load.h>
#include <assimp/postprocess.h>
#include <SDL.h>

#undef main

void import()
{
    elf_import_model(
        "",
        "",
        aiProcessPreset_TargetRealtime_Quality);
    elf_import_shader(
        NULL,
        "",
        NULL,
        NULL,
        NULL,
        "");
}

void load()
{
    elf_loader loader;
    elf_load_res_shader(&loader, "");
    elf_load_res_vao(&loader, "");
}

int main()
{
    return 0;
}