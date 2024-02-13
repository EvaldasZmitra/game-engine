#include <elf_main.h>
#include <elf_math.h>

#undef main

int main()
{
    ElfWindow window = elf_window_init();
    SimpleShader shader = elf_main_shader_create();
    ElfModel model = elf_model_load("./resources/Sphere.fbx");

    float position[3] = {0, 0, 1};
    float scale[3] = {0.5, 0.5, 0.5};
    float rotation[3] = {0, 0, 0};
    float camera_position[3] = {0, 0, 0};
    float center[3] = {0, 0, 1};
    float up[3] = {0, 1, 0};
    float model_matrix[16];
    float projection_matrix[16];
    float view_matrix[16];
    elf_math_transform_create(
        model_matrix,
        rotation,
        position,
        scale);
    elf_math_project(
        projection_matrix,
        60,
        8.0f / 6.0f,
        0.1f,
        100.0f);
    elf_math_view(
        view_matrix,
        camera_position,
        center,
        up);
    while (running)
    {
        elf_window_poll(&window, elf_main_handle_event);
        elf_window_clear(&window);

        elf_main_shader_use(&shader);
        elf_shader_set_matrix4x4(shader.shader_id, "model", model_matrix);
        elf_shader_set_matrix4x4(shader.shader_id, "view", view_matrix);
        elf_shader_set_matrix4x4(shader.shader_id, "projection", projection_matrix);
        elf_model_draw(&model);

        elf_window_update(&window);
    }

    elf_model_free(&model);
    elf_main_shader_destroy(&shader);

    elf_window_quit(&window);
}
