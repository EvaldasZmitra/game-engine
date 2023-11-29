#include <elf_window.h>

ElfWindow elf_window_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
    }

    SDL_Window *window = SDL_CreateWindow("OpenGL Triangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW initialization failed\n");
    }

    ElfWindow elf_window = {
        .window = window};
    return elf_window;
}

void elf_window_quit(const ElfWindow *window)
{
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}

void elf_window_clear(const ElfWindow *window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void elf_window_update(const ElfWindow *window)
{
    SDL_GL_SwapWindow(window->window);
}

void elf_window_poll(const ElfWindow *window, ElfWindowEventAction action)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        action(event);
    }
}
