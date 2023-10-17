#include <elf_render_object.h>

void elf_render_object_render(ElfRenderObject *object)
{
    elf_shader_use(object->shader);
    glDrawArrays(GL_TRIANGLES, 0, object->count);
}
