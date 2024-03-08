#include <elf_import.h>
#include <assimp/postprocess.h>
#include <elf_str.h>
#include <elf_io.h>
#include <SDL.h>
#undef main

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow(
        "name",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600,
        600,
        SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW initialization failed\n");
    }

    elf_str replace = elf_str_init_null(
        "C:\\Projects\\game-engine\\newest\\res",
        1024);
    elf_str path = elf_str_init_null("C:\\Projects\\game-engine\\newest\\src\\model.fbx", 1024);
    elf_str shader_out = elf_str_init_null(
        "C:\\Projects\\game-engine\\newest\\res\\shd\\test.bin",
        1024);
    elf_load_program(
        &shader_out,
        NULL,
        elf_load_vert(elf_read("C:\\Projects\\game-engine\\newest\\src\\shaders\\test_shader.vert")),
        NULL,
        NULL,
        NULL,
        elf_load_frag(elf_read("C:\\Projects\\game-engine\\newest\\src\\shaders\\test_shader.frag")));
    elf_load_model(
        &path,
        &replace,
        aiProcessPreset_TargetRealtime_MaxQuality);
    elf_str_free(&replace);
    return 0;
}