#include "frame.h"
#include "paging.h"

void set_frame(u32int frame_addr)
{
    u32int frame = frame_addr / PAGE_SIZE;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << offset);
}

void clear_frame(u32int frame_addr)
{
    u32int frame = frame_addr / PAGE_SIZE;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << offset);
}

u32int test_frame(u32int frame_addr)
{
    u32int frame = frame_addr / PAGE_SIZE;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << offset));
}

u32int get_free_frame()
{
    u32int i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xFFFFFFFF)
        {
            for (j = 0; j < 32; j++)
            {
                u32int frame = i * 8 * 4 + j;
                if (!(test_frame(frame * PAGE_SIZE)))
                {
                    return frame;
                }
            }
        }
    }
    return (u32int)-1;
}