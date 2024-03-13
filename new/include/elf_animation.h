#ifndef ELF_ANIMATION_H
#define ELF_ANIMATION_H

#include <GL/glew.h>

typedef struct elf_animation
{
    GLuint num_position_keys;
    GLfloat *position_times;
    GLuint *position_keys;

    GLuint num_rotation_keys;
    GLfloat *rotation_times;
    GLuint *rotation_keys;

    GLuint num_scale_keys;
    GLfloat *scale_times;
    GLuint *scale_keys;
} elf_animation;

#endif