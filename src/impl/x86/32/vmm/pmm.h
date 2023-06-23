#include "common.h"

#define BLOCK_SIZE 0x1000 // size of 1 memory block, 4MB

void set_block(u32int bit);

void clear_block(u32int bit);

s32int find_first_free_blocks(u32int num_blocks);

u32int *allocate_blocks(u32int num_blocks);

void free_blocks(u32int *address, u32int num_blocks);

void pmm_init(u32int start_addr, u32int size);