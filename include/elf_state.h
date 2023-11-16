#ifndef ELF_STATE_H
#define ELF_STATE_H

#include <elf_entity.h>

typedef struct ElfState
{
    ElfEntity *entities;
    int num_entities;
} ElfState;

#endif