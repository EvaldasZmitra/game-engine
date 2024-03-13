#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>
#include <elf_import.h>
#include <assimp/postprocess.h>

#undef main

int main()
{
    SDL_Window *window = SDL_CreateWindow("title", 0, 0, 1, 1, SDL_WINDOW_OPENGL);
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

    char *res = "C:\\Projects\\game-engine\\elf\\build\\res";
    elf_import_shader(
        res,
        "default",
        "C:\\Projects\\game-engine\\elf-import\\res\\default.vert",
        "C:\\Projects\\game-engine\\elf-import\\res\\default.frag");
    elf_import_model(
        "C:\\Projects\\game-engine\\elf-import\\res\\vampire.fbx",
        res,
        aiProcessPreset_TargetRealtime_MaxQuality);

    SDL_Quit();
    return 0;
}