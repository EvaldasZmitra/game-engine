#include <elf_main.h>
#include <elf_math.h>
#include <elf_loader.h>
#include <cglm/cglm.h>
#include <SDL.h>
#include <SDL_image.h>
#include <elf_material.h>
#include <elf_camera.h>
#include <elf.h>

#undef main

int main()
{
    elf_init();
    elf_camera camera = elf_cam_init();
    camera.zFar = 1000;
    elf_cam_update(&camera);
    ElfWindow window = elf_window_init("Window", 800, 800);
    ElfModels models = elf_model_load("./resources/wizzard.fbx");
    for (int i = 0; i < models.num_models; i++)
    {
        models.models[i].transform = (ElfTransform){
            .position = (vec3){0, -25, 100},
            .rotation = (vec3){-90, 0, 180},
            .scale = (vec3){1, 1, 1},
        };
    }
    unsigned int wizz_texture = elf_load_texture("C:\\Projects\\game-engine\\resources\\wiz.jpg");
    unsigned int shader = elf_shader_create(
        elf_io_read("./resources/test_shader.vert"),
        elf_io_read("./resources/test_shader.frag"));
    elf_texture texture = {
        .name = "myTextureSampler",
        .texture_id = wizz_texture};
    elf_texture textures[] = {texture};
    elf_material material = {
        .num_textures = 1,
        .shader_id = shader,
        .textures = textures};
    while (running)
    {
        elf_window_poll(&window, elf_main_handle_event);
        elf_window_clear(&window);
        elf_model_use_material(&material);
        elf_cam_bind(&camera, shader);
        for (size_t i = 0; i < models.num_models; i++)
        {
            ElfModel model = models.models[i];
            elf_model_draw(&model, shader);
        }
        elf_window_update(&window);
    }
    elf_window_quit(&window);
}