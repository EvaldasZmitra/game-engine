#ifndef ELF_WINDOW_H
#define ELF_WINDOW_H

#include <SDL.h>
#include <GL/glew.h>

typedef struct ElfWindow
{
    SDL_Window *window;
} ElfWindow;

typedef void (*ElfWindowEventAction)(SDL_Event);

ElfWindow elf_window_init();
void elf_window_quit(const ElfWindow* window);
void elf_window_clear(const ElfWindow* window);
void elf_window_update(const ElfWindow* window);
void elf_window_poll(const ElfWindow* window, ElfWindowEventAction action);

#endif