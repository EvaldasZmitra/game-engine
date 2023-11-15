#include <systems.h>

void advance_systems(Systems *systems, State *state)
{
    for (int i = 0; i < systems->num_systems; i++)
    {
        System system = systems->systems[i];
        advance_system(&system, state);
    }
}
