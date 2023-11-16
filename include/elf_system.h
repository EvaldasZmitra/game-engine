#ifndef ELF_SYSTEM_H
#define ELF_SYSTEM_H

#include <elf_state.h>
#include <elf_thread_pool.h>
#include <elf_bitmask_256.h>

typedef struct ElfSystem
{
    ElfBitMask256 *required_components;
    void (*advance)(ElfEntity *entity);
} ElfSystem;

void elf_system_advance(ElfSystem *system, ElfState *state, ElfThreadPool *thread_pool);

#endif