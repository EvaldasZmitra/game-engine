#ifndef ELF_WINDOW_H
#define ELF_WINDOW_H

#include <SDL.h>
#include <GL/glew.h>

typedef struct elf_window
{
    SDL_Window *window;
} elf_window;

typedef void (*ElfWindowEventAction)(SDL_Event);

elf_window elf_window_init(
    const char *name,
    unsigned int width,
    unsigned int height);
void elf_window_quit(const elf_window *window);
void elf_window_clear(const elf_window *window);
void elf_window_update(const elf_window *window);
void elf_window_poll(const elf_window *window, ElfWindowEventAction action);

#endif