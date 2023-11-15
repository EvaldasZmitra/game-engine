#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <system.h>

typedef struct Systems
{
    System *systems;
    int num_systems;
} Systems;

void advance_systems(Systems *systems, State *state);

#endif