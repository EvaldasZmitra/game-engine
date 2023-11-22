#include <Gl/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <glib.h>
#include <elf_window.h>
#include <elf_shader.h>
#include <elf_io.h>
#include <elf_render_object.h>
#include <elf_state.h>
#include <elf_systems.h>

#undef main

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

int running = 1;

float vertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f};

void handle_elf_event(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        running = 0;
    }
}

void my_system_advance(ElfEntity *entity)
{
}

int main()
{
    ElfBitMask256 bitmask;
    elf_bitmask_256_set(&bitmask, 0);
    int component1 = 5;
    int num_entities = 100;
    ElfEntity entities[100];
    for (int i = 0; i < num_entities; i++)
    {
        ElfEntity entity = {
            .id = i,
            .components = {&component1},
            .components_mask = &bitmask};
        entities[i] = entity;
    }

    ElfState state = {
        .entities = entities,
        .num_entities = num_entities};
    ElfSystems systems = {
        .num_systems = 1,
        .systems = (ElfSystem[]){
            {
                .required_components = &bitmask,
                .advance = my_system_advance,
            }}};

    ElfWindow window = elf_window_init();
    ElfShader shader = elf_shader_create(
        elf_io_read("./resources/test_shader.vert"),
        elf_io_read("./resources/test_shader.frag"));
    elf_shader_add_vec3_data(
        &shader,
        vertices,
        3,
        0);
    ElfRenderObject triangle = {
        .shader = &shader,
        .count = 3};
    while (running)
    {
        elf_window_poll(&window, handle_elf_event);
        elf_window_clear(&window);
        elf_render_object_render(&triangle);
        elf_window_update(&window);
        elf_systems_advance(&systems, &state);
    }

    elf_shader_free(&shader);
    elf_window_quit(&window);
}
