#include <GL/glew.h>
#include <elf_window.h>

SDL_Window *elf_window_create(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    unsigned int flags)
{
    SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!window)
    {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
    }
    SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW initialization failed\n");
    }
    return window;
}
