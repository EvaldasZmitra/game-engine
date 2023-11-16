#ifndef ELF_SYSTEMS_H
#define ELF_SYSTEMS_H

#include <elf_system.h>

typedef struct ElfSystems
{
    ElfSystem *systems;
    int num_systems;
    ElfThreadPool *thread_pool;
} ElfSystems;

void elf_systems_advance(ElfSystems *systems, ElfState *state);

#endif