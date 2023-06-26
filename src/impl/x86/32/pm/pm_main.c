#include "pm.h"

static u32int *memory_map = 0;
static u32int max_blocks = 0;
static u32int used_blocks = 0;

void set_block(u32int bit)
{
    // Divide bit by 32 to get 32bit "chunk" of memory aining bit to be set;
    // Shift 1 by remainder of bit divided by 32 to get bit to set within the
    //   32 bit chunk
    // OR to set that bit
    memory_map[bit / 32] |= (1 << (bit % 32));
}

void unset_block(u32int bit)
{
    // Divide bit by 32 to get 32bit "chunk" of memory aining bit to be set;
    // Shift 1 by remainder of bit divided by 32 to get bit to set within the
    //   32 bit chunk
    // AND with the inverse of those bits to clear the bit to 0
    memory_map[bit / 32] &= ~(1 << (bit % 32));
}

// Test if a block/bit in the memory map is set/used
// uint8_t test_block( u32int bit)
//{
// Divide bit by 32 to get 32bit "chunk" of memory aining bit to be set;
// Shift 1 by remainder of bit divided by 32 to get bit within the 32bit chunk;
// AND and return that bit (will be 1 or 0)
//   return memory_map[bit/32] & (1 << (bit % 32));
//}

s32int find_first_free_blocks(u32int num_blocks)
{
    if (num_blocks == 0)
        return -1; // Can't return no memory, error

    // Test 32 blocks at a time
    for (u32int i = 0; i < max_blocks / 32; i++)
    {
        if (memory_map[i] != 0xFFFFFFFF)
        {
            // At least 1 bit is not set within this 32bit chunk of memory,
            //   find that bit by testing each bit
            for (s32int j = 0; j < 32; j++)
            {
                s32int bit = 1 << j;

                // If bit is unset/0, found start of a free region of memory
                if (!(memory_map[i] & bit))
                {
                    for (u32int count = 0, free_blocks = 0; count < num_blocks; count++)
                    {
                        if ((j + count > 31) && (i + 1 <= max_blocks / 32))
                        {
                            if (!(memory_map[i + 1] & (1 << ((j + count) - 32))))
                                free_blocks++;
                        }
                        else
                        {
                            if (!(memory_map[i] & (1 << (j + count))))
                                free_blocks++;
                        }

                        if (free_blocks == num_blocks) // Found enough free space
                            return i * 32 + j;
                    }
                }
            }
        }
    }

    return -1; // No free region of memory large enough
}

u32int *allocate_blocks(u32int num_blocks)
{
    // If # of free blocks left is not enough, we can't allocate any more, return
    if ((max_blocks - used_blocks) <= num_blocks)
        return 0;

    s32int starting_block = find_first_free_blocks(num_blocks);
    if (starting_block == -1)
        return 0; // Couldn't find that many blocks in a row to allocate

    // Found free blocks, set them as used
    for (u32int i = 0; i < num_blocks; i++)
        set_block(starting_block + i);

    used_blocks += num_blocks; // Blocks are now used/reserved, increase count

    // Convert blocks to bytes to get start of actual RAM that is now allocated
    u32int address = starting_block * BLOCK_SIZE;

    return (u32int *)address; // Physical memory location of allocated blocks
}

void free_blocks(u32int *address, u32int num_blocks)
{
    s32int starting_block = (u32int)address / BLOCK_SIZE; // Convert address to blocks

    for (u32int i = 0; i < num_blocks; i++)
        unset_block(starting_block + i); // Unset bits/blocks in memory map, to free

    used_blocks -= num_blocks; // Decrease used block count
}

void pm_init(u32int start_addr, u32int size)
{
    memory_map = (u32int *)start_addr;
    max_blocks = size / BLOCK_SIZE;
    used_blocks = max_blocks; // To start off, every block will be "used/reserved"

    // By default, set all memory in use (used blocks/bits = 1, every block is set)
    // Each byte of memory map holds 8 bits/blocks
    memset(memory_map, 0xF, max_blocks / BLOCKS_PER_BYTE);
}

void pm_init_region(u32int base_addr, u32int size)
{
    u32int align = base_addr / BLOCK_SIZE;
    u32int blocks = size / BLOCK_SIZE;

    for (; blocks > 0; blocks--)
    {
        unset_block(align++);
        used_blocks--;
    }
    set_block(0);
}

void pm_deinit_region(u32int base_addr, u32int size)
{
    u32int align = base_addr / BLOCK_SIZE;
    u32int blocks = size / BLOCK_SIZE;

    for (; blocks > 0; blocks--)
    {
        set_block(align++);
        used_blocks++;
    }
    set_block(0);
}