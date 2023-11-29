#include <elf_system.h>
#include <omp.h>
#include <stdio.h>

typedef struct AdvanceSystemThreadParams
{
    ElfSystem *system;
    ElfState *state;
    int start;
    int end;
} AdvanceSystemThreadParams;

void elf_system_advance(ElfSystem *system, ElfState *state)
{
#pragma omp parallel
#pragma omp for
    for (int i = 0; i < state->num_entities; i++)
    {
        ElfEntity *entity = &state->entities[i];
        if (elf_bitmask_256_contains(entity->components_mask, system->required_components))
        {
            system->advance(entity);
        }
    }
}
