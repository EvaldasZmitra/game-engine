#ifndef ELF_MAIN_H
#define ELF_MAIN_H

#include <elf_window.h>
#include <elf_shader.h>
#include <elf_io.h>
#include <elf_state.h>
#include <elf_systems.h>
#include <elf_model.h>

int running = 1;

typedef struct SimpleShader
{
    unsigned int shader_id;
} SimpleShader;

void elf_main_handle_event(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        running = 0;
    }
}

SimpleShader elf_main_shader_create()
{
    SimpleShader shader;
    shader.shader_id = elf_shader_create(
        elf_io_read("./resources/test_shader.vert"),
        elf_io_read("./resources/test_shader.frag"));
    return shader;
}

void elf_main_shader_use(SimpleShader *shader)
{
    elf_shader_use(shader->shader_id);
}

void elf_main_shader_destroy(SimpleShader *shader)
{
    elf_shader_destroy(shader->shader_id);
}

#endif