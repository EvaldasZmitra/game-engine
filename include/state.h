#ifndef STATE_H
#define STATE_H

#include <entity.h>

typedef struct State
{
    Entity *entities;
    int num_entities;
} State;

#endif