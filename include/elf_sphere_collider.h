#ifndef ELF_COLLIDER
#define ELF_COLLIDER

typedef struct ElfSphereCollider
{
    unsigned int radius;

    unsigned int position_x;
    unsigned int position_y;
    unsigned int position_z;

    unsigned int velocity_x;
    unsigned int velocity_y;
    unsigned int velocity_z;
} ElfSphereCollider;

void advance(ElfSphereCollider *collider, int time_step);
void scan_collisions(
    ElfSphereCollider *collider,
    ElfSphereCollider **colliders,
    unsigned int num_colliders);

#endif