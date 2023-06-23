#include "pm.h"

static u32int *memory_map = 0;
static u32int max_blocks = 0;
static u32int used_blocks = 0;

u32int *allocate_blocks(u32int num_blocks)
{
    used_blocks += num_blocks;
    max_blocks = used_blocks;
    return memory_map;
}

void free_blocks(u32int *address, u32int num_blocks)
{
    memory_map = address;
    used_blocks -= num_blocks;
}
