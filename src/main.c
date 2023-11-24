#include <Gl/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <glib.h>
#include <elf_window.h>
#include <elf_shader.h>
#include <elf_io.h>
#include <elf_render_object.h>
#include <elf_state.h>
#include <elf_systems.h>
#include <elf_model.h>
#include <stdlib.h>
#include <memory.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#undef main

int running = 1;

void handle_elf_event(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        running = 0;
    }
}

void my_system_advance(ElfEntity *entity)
{
}

int main()
{
    ElfWindow window = elf_window_init();
    ElfShader shader = elf_shader_create(
        elf_io_read("./resources/test_shader.vert"),
        elf_io_read("./resources/test_shader.frag"));
    ElfModel model = elf_model_load("./resources/Sphere.fbx");

    while (running)
    {
        elf_window_poll(&window, handle_elf_event);
        elf_window_clear(&window);

        glUseProgram(shader.id);
        elf_model_draw(&model);

        elf_window_update(&window);
    }

    elf_model_free(&model);
    elf_shader_free(&shader);
    elf_window_quit(&window);
}
