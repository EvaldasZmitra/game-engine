#ifndef ENTITY_H
#define ENTITY_H

#include <component.h>

typedef struct Entity
{
    int id;
    Component *components;
    int num_components;
} Entity;

#endif