#include <elf_camera.h>
#include <elf_shader.h>

void elf_cam_bind(elf_camera *camera, unsigned int shader)
{
    elf_shader_set_matrix4x4(shader, "v", &camera->view[0][0]);
    elf_shader_set_matrix4x4(shader, "p", &camera->proj[0][0]);
}

void elf_cam_update(elf_camera *camera)
{
    glm_lookat(
        camera->pos,
        camera->target,
        camera->up,
        camera->view);
    glm_perspective(
        glm_rad(camera->fov),
        camera->aspect,
        camera->zNear,
        camera->zFar,
        camera->proj);
}

elf_camera elf_cam_init()
{
    elf_camera camera = {
        .fov = 45.0f,
        .zNear = 0.1f,
        .zFar = 100.0f,
        .view = GLM_MAT4_IDENTITY_INIT,
        .proj = GLM_MAT4_IDENTITY_INIT,
        .aspect = 1};
    camera.pos[0] = 0;
    camera.pos[1] = 0;
    camera.pos[2] = 0;

    camera.target[0] = 0;
    camera.target[1] = 0;
    camera.target[2] = 1;

    camera.up[0] = 0;
    camera.up[1] = 1;
    camera.up[2] = 0;

    camera.fov = 45;
    elf_cam_update(&camera);
    return camera;
}