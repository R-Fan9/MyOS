#ifndef PM_H
#define PM_H

#include "common.h"

#define BLOCK_SIZE 0x1000 // size of 1 memory block, 4KB
#define BLOCKS_PER_BYTE 8

// Sets a block/bit in the memory map (mark block as used)
void set_block(u32int bit);

// Unsets a block/bit in the memory map (mark block as free)
void unset_block(u32int bit);

// Find the first free blocks of memory for a given size
s32int find_first_free_blocks(u32int num_blocks);

// Allocate blocks of memory
u32int *allocate_blocks(u32int num_blocks);

// Free blocks memory
void free_blocks(u32int *address, u32int num_blocks);

void pm_init(u32int start_addr, u32int size);

void pm_init_region(u32int base_addr, u32int size);

void pm_deinit_region(u32int base_addr, u32int size);

#endif