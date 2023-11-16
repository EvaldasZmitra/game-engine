#include <elf_system.h>
#include <elf_math.h>

typedef struct AdvanceSystemThreadParams
{
    ElfSystem *system;
    ElfState *state;
    int start;
    int end;
} AdvanceSystemThreadParams;

void advance_system_per_thread(void *args)
{
    AdvanceSystemThreadParams *params = (AdvanceSystemThreadParams *)args;
    for (int i = params->start; i < params->end; i++)
    {
        ElfEntity *entity = &params->state->entities[i];
        ElfSystem *system = params->system;
        if (elf_bitmask_256_contains(entity->components_mask, system->required_components))
        {
            system->advance(entity);
        }
    }
}

void elf_system_advance(ElfSystem *system, ElfState *state, ElfThreadPool *thread_pool)
{
    int num_entities_per_thread = elf_math_divide_round_up(
        state->num_entities,
        thread_pool->num_threads);
    for (int i = 0; i < state->num_entities; i += num_entities_per_thread)
    {
        AdvanceSystemThreadParams params = {
            .system = system,
            .state = state,
            .start = i,
            .end = elf_math_min(i + num_entities_per_thread, state->num_entities)};
        elf_thread_pool_add_work(thread_pool, advance_system_per_thread, &params);
    }
}
