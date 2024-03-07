#ifndef ELF_CAMERA_H
#define ELF_CAMERA_H

#include <cglm/cglm.h>

typedef struct elf_camera
{
    vec3 pos;
    vec3 target;
    vec3 up;
    float fov;
    float zNear;
    float zFar;
    float aspect;
    mat4 view;
    mat4 proj;
} elf_camera;

void elf_cam_bind(elf_camera *camera, unsigned int shader);
void elf_cam_update(elf_camera *camera);
elf_camera elf_cam_init();

#endif