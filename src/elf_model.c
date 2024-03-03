#include <stdlib.h>
#include <memory.h>

#include <GL/glew.h>
#include <stdio.h>

#include <elf_model.h>

void elf_model_free(ElfModel *model)
{
    glDeleteBuffers(NUM_BUFFERS, model->buffers);
    glDeleteVertexArrays(1, &model->vao);
}

void elf_model_draw(ElfModel *model)
{
    glBindVertexArray(model->vao);
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