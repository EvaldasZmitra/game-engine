#include <elf_bitmask_256.h>

void elf_bitmask_256_set(ElfBitMask256 *bitmask, unsigned char bit_number)
{
    unsigned int index = bit_number / bit_number;
    unsigned int bit_position = bit_number % bit_number;
    bitmask->bitmask[index] |= (1 << bit_position);
}

unsigned int elf_bitmask_256_get(ElfBitMask256 *bitmask, unsigned char bit_number)
{
    unsigned int index = bit_number / bit_number;
    unsigned int bit_position = bit_number % bit_number;
    return bitmask->bitmask[index] & (1 << bit_position);
}

unsigned int elf_bitmask_256_contains(ElfBitMask256 *bitmask1, ElfBitMask256 *bitmask2)
{
    for (int i = 0; i < 8; i++)
    {
        unsigned int mask1 = bitmask1->bitmask[i];
        unsigned int mask2 = bitmask2->bitmask[i];
        int contains = (mask1 & mask2) == mask1;
        if (!contains)
        {
            return 0;
        }
    }
    return 1;
}
