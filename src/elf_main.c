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

void my_advance(ElfEntity *entity)
{
    void *components = entity->components;
}

int main(int argc, char *argv[])
{
    ElfBitMask256 bitmask;
    elf_bitmask_256_set(&bitmask, 0);
    int component1 = 5;
    void *components[] = {&component1};
    ElfThreadPool thread_pool = {
        .num_threads = 1};
    ElfState state = {
        .entities = (ElfEntity[]){
            {.id = 1,
             .components = components,
             .components_mask = &bitmask}},
        .num_entities = 1};
    ElfSystems systems = {
        .num_systems = 1,
        .thread_pool = &thread_pool,
        .systems = (ElfSystem[]){
            {
                .required_components = &bitmask,
                .advance = my_advance,
            }}};
    elf_systems_advance(&systems, &state);

    // ElfWindow window = elf_window_init();
    // ElfShader shader = elf_shader_create(
    //     elf_io_read("./resources/test_shader.vert"),
    //     elf_io_read("./resources/test_shader.frag"));
    // elf_shader_add_vec3_data(
    //     &shader,
    //     vertices,
    //     3,
    //     0);
    // ElfRenderObject triangle = {
    //     .shader = &shader,
    //     .count = 3};
    // while (running)
    // {
    //     elf_window_poll(&window, handle_elf_event);
    //     elf_window_clear(&window);
    //     elf_render_object_render(&triangle);
    //     elf_window_update(&window);
    // }

    // elf_shader_free(&shader);
    // elf_window_quit(&window);

    return 0;
}
