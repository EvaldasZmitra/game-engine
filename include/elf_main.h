#ifndef ELF_MAIN_H
#define ELF_MAIN_H

#include <elf_window.h>
#include <elf_shader.h>
#include <elf_io.h>
#include <elf_state.h>
#include <elf_systems.h>
#include <elf_model.h>

int running = 1;

void elf_main_handle_event(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        running = 0;
    }
}

#endif