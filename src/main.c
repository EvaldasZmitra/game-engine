#include <Gl/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <glib.h>
#include <elf_window.h>
#include <elf_shader.h>
#include <elf_util.h>
#include <elf_render_object.h>

#undef main

int running = 1;

float vertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f};

void handle_elf_event(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        running = 0;
    }
}

int main(int argc, char *argv[])
{
    ElfWindow window = elf_window_init();
    ElfShader shader = elf_shader_create(
        elf_util_read("./resources/test_shader.vert"),
        elf_util_read("./resources/test_shader.frag"));
    elf_shader_add_vec3_data(
        &shader,
        vertices,
        3,
        0);
    ElfRenderObject triangle = {
        .shader = &shader,
        .count = 3};
    while (running)
    {
        elf_window_poll(&window, handle_elf_event);
        elf_window_clear(&window);
        elf_render_object_render(&triangle);
        elf_window_update(&window);
    }

    elf_shader_free(&shader);
    elf_window_quit(&window);

    return 0;
}
