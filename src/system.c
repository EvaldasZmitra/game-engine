#include <system.h>

void advance_system(System *system, State *state)
{
    for (int i = 0; i < state->num_entities; i++)
    {
        Entity entity = state->entities[i];
        if (does_entity_fit(&entity, system))
        {
            system->advance(&entity);
        }
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
