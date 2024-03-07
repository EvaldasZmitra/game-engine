#include <stdlib.h>
#include <memory.h>
#include <GL/glew.h>
#include <stdio.h>
#include <elf_model.h>
#include <cglm/cglm.h>

void elf_model_bind_shader(ElfModel *model, unsigned int shader)
{
    mat4 modelm = GLM_MAT4_IDENTITY_INIT;
    glm_translate(modelm, model->transform.position);
    glm_rotate(modelm, glm_rad(model->transform.rotation[0]), (vec3){1, 0, 0});
    glm_rotate(modelm, glm_rad(model->transform.rotation[1]), (vec3){0, 1, 0});
    glm_rotate(modelm, glm_rad(model->transform.rotation[2]), (vec3){0, 0, 1});
    glm_scale(modelm, model->transform.scale);
    elf_shader_set_matrix4x4(shader, "m", &modelm[0][0]);
}

void elf_model_free(ElfModel *model)
{
    glDeleteBuffers(NUM_BUFFERS, model->buffers);
    glDeleteVertexArrays(1, &model->vao);
}

void elf_model_draw(ElfModel *model, unsigned int shader)
{
    glBindVertexArray(model->vao);
    mat4 modelm = GLM_MAT4_IDENTITY_INIT;
    glm_translate(modelm, model->transform.position);
    glm_rotate(modelm, glm_rad(model->transform.rotation[0]), (vec3){1, 0, 0});
    glm_rotate(modelm, glm_rad(model->transform.rotation[1]), (vec3){0, 1, 0});
    glm_rotate(modelm, glm_rad(model->transform.rotation[2]), (vec3){0, 0, 1});
    glm_scale(modelm, model->transform.scale);
    elf_shader_set_matrix4x4(shader, "m", &modelm[0][0]);
    glDrawElements(GL_TRIANGLES, model->num_indices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void elf_model_draw_instanced(ElfModel *model, int num_instances)
{
    glBindVertexArray(model->vao);
    glDrawElementsInstanced(GL_TRIANGLES, model->num_indices, GL_UNSIGNED_INT, NULL, num_instances);
    glBindVertexArray(0);
}

void elf_model_use_material(ElfMaterial *elf_material)
{
    glUseProgram(elf_material->shader);
    for (unsigned int i = 0; i < elf_material->num_textures; i++)
    {
        ElfTexture texture = elf_material->textures[i];
        int uniform_id = glGetUniformLocation(elf_material->shader, texture.name);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture.texture_id);
        glUniform1i(uniform_id, i);
    }
}