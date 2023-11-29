#ifndef ELF_BITMASK_256
#define ELF_BITMASK_256

typedef struct ElfBitMask256
{
    unsigned int bitmask[8];
} ElfBitMask256;

void elf_bitmask_256_set(ElfBitMask256* bitmask, unsigned char bit_number);
unsigned int elf_bitmask_256_get(ElfBitMask256* bitmask, unsigned char bit_number);
unsigned int elf_bitmask_256_contains(ElfBitMask256* bitmask1, ElfBitMask256* bitmask2);

#endif