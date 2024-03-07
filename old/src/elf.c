#include <elf.h>
#include <SDL.h>
#include <SDL_image.h>

void elf_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
    }
    IMG_Init(IMG_INIT_JPG);
    IMG_Init(IMG_INIT_PNG);
}
