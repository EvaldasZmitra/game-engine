#include <elf_systems.h>

void elf_systems_advance(ElfSystems *systems, ElfState *state)
{
    for (int i = 0; i < systems->num_systems; i++)
    {
        ElfSystem *system = &systems->systems[i];
        elf_system_advance(system, state, systems->thread_pool);
    }
}
