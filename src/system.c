#include <system.h>
#include <math.h>

void advance_system(System *system, State *state, ThreadPool *thread_pool)
{
    int num_entities_per_thread = state->num_entities / thread_pool->num_threads;
    for (int i = 0; i < state->num_entities; i += num_entities_per_thread)
    {
        advance_system_thread(system, state, i, i + num_entities_per_thread);
    }

    for (int i = 0; i < state->num_entities; i++)
    {
        Entity entity = state->entities[i];
        if (does_entity_fit(&entity, system))
        {
            system->advance(&entity);
        }
    }
}

void advance_system_thread(System *system, State *state, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        Entity *entity = &state->entities[i];
        system->advance(entity);
    }
}

int does_entity_fit(Entity *entity, System *system)
{
    int num_matching_components = 0;
    for (int i = 0; i < entity->num_components; i++)
    {
        Component component = entity->components[i];
        for (int y = 0; y < system->num_required_components; y++)
        {
            int required_component_id = system->required_component_ids[y];
            if (required_component_id == component.id)
            {
                num_matching_components++;
                if (num_matching_components == system->num_required_components)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
