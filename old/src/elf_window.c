#include <elf_window.h>

elf_window elf_window_init(
    const char *name,
    unsigned int width,
    unsigned int height)
{
    SDL_Window *window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL);
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

    elf_window elf_window = {
        .window = window};

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    return elf_window;
}

void elf_window_quit(const elf_window *window)
{
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}

void elf_window_clear(const elf_window *window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void elf_window_update(const elf_window *window)
{
    SDL_GL_SwapWindow(window->window);
}

void elf_window_poll(const elf_window *window, ElfWindowEventAction action)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        action(event);
    }
}
