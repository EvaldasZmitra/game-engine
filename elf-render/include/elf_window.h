#ifndef ELF_WINDOW_H
#define ELF_WINDOW_H

#include <SDL.h>

SDL_Window *elf_window_create(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    unsigned int flags);

#endif
