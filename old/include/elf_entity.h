#ifndef ELF_ENTITY_H
#define ELF_ENTITY_H

#include <elf_bitmask_256.h>

typedef struct ElfEntity
{
    unsigned int id;
    void *components[256];
    ElfBitMask256 *components_mask;
} ElfEntity;

void *elf_entity_get_component(ElfEntity *entity, unsigned char component_id);

void elf_entity_set_component(
    ElfEntity *entity,
    unsigned char component_id,
    void *component);

#endif