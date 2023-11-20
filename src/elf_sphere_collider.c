#include <elf_sphere_collider.h>

void advance(ElfSphereCollider *collider, int time_step)
{
    collider->position_x += collider->position_x * time_step;
    collider->position_y += collider->position_y * time_step;
    collider->position_z += collider->position_z * time_step;
}

unsigned int square_distance_between(ElfSphereCollider *collider, ElfSphereCollider *other)
{
    unsigned int distance_x = collider->position_x - other->position_x;
    unsigned int distance_y = collider->position_x - other->position_y;
    unsigned int distance_z = collider->position_x - other->position_z;
    distance_x *= distance_x;
    distance_y *= distance_y;
    distance_z *= distance_z;
}

void scan_collisions(
    ElfSphereCollider *collider,
    ElfSphereCollider **colliders,
    unsigned int num_colliders)
{
    for (unsigned int i = 0; i < num_colliders; i++)
    {
        ElfSphereCollider *other = colliders[i];
        unsigned int distance_squared = square_distance_between(collider, other);
        unsigned int max_distance = other->radius + collider->radius;
        if (distance_squared < max_distance * max_distance)
        {
            // collision
        }
    }
}
