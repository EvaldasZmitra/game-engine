#include <elf_main.h>
#include <elf_math.h>
#include <elf_loader.h>
#include <cglm/cglm.h>
#include <SDL.h>
#include <SDL_image.h>
#include <elf_material.h>

#undef main

int main()
{
    ElfWindow window = elf_window_init();
    ElfModels models = elf_model_load("./resources/sphere.obj");

    mat4 view;
    mat4 projection;
    glm_lookat(
        (vec3){0, 0, 0},
        (vec3){0, 0, 1},
        (vec3){0, 1, 0},
        view);
    glm_perspective(
        glm_rad(45.0f),
        1,
        0.1,
        100,
        projection);

    IMG_Init(IMG_INIT_PNG);
    IMG_Init(IMG_INIT_JPG);

    unsigned int wizz_texture = elf_load_texture("C:\\Projects\\game-engine\\resources\\wiz.jpg");
    unsigned int shader = elf_shader_create(
        elf_io_read("./resources/test_shader.vert"),
        elf_io_read("./resources/test_shader.frag"));
    ElfTexture texture = {
        .name = "myTextureSampler",
        .texture_id = wizz_texture};
    ElfTexture textures[] = {
        texture};

    ElfMaterial material = {
        .num_textures = 1,
        .shader = shader,
        .textures = textures};

    float i = 0;
    while (running)
    {
        i += 0.5f;
        mat4 modelm = GLM_MAT4_IDENTITY_INIT;
        glm_translate(modelm, (vec3){0, 0, 10});
        glm_rotate(modelm, glm_rad(i), (vec3){0, 1, 0});
        glm_scale(modelm, (vec3){1, 1, 1});

        elf_window_poll(&window, elf_main_handle_event);
        elf_window_clear(&window);

        elf_model_use_material(&material);
        elf_shader_set_matrix4x4(shader, "m", &modelm[0][0]);
        elf_shader_set_matrix4x4(shader, "v", &view[0][0]);
        elf_shader_set_matrix4x4(shader, "p", &projection[0][0]);

        for (size_t i = 0; i < models.num_models; i++)
        {
            ElfModel model = models.models[i];
            elf_model_draw(&model);
        }

        elf_window_update(&window);
    }

    elf_window_quit(&window);
}

// SDL_Surface *img = IMG_Load("C:\\Projects\\game-engine\\resources\\wiz.jpg");
// GLuint textureID;

// int Mode = GL_RGB;
// if (img->format->BytesPerPixel == 4)
// {
//     Mode = GL_RGBA;
// }
// glGenTextures(1, &textureID);
// glBindTexture(GL_TEXTURE_2D, textureID);
// glTexImage2D(GL_TEXTURE_2D, 0, Mode, img->w, img->h, 0, Mode, GL_UNSIGNED_BYTE, img->pixels);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);