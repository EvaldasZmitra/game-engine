#ifndef SYSTEM_H
#define SYSTEM_H

#include <component.h>
#include <state.h>
#include <thread_pool.h>

typedef struct System
{
    int *required_component_ids;
    int num_required_components;
    void (*advance)(Entity *entity);
} System;

void advance_system(System *system, State *state, ThreadPool *thread_pool);
int does_entity_fit(Entity *entity, System *system);

#endif