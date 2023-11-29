#include <elf_main.h>

#undef main

int main()
{
    ElfWindow window = elf_window_init();
    SimpleShader shader = elf_main_shader_create();
    ElfModel model = elf_model_load("./resources/Sphere.fbx");

    while (running)
    {
        elf_window_poll(&window, elf_main_handle_event);
        elf_window_clear(&window);

        elf_main_shader_use(&shader);
        elf_model_draw(&model);

        elf_window_update(&window);
    }

    elf_model_free(&model);
    elf_main_shader_destroy(&shader);
    elf_window_quit(&window);
}
