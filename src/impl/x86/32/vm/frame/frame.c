#include "frame.h"
u32int *frames;
u32int nframes;

void set_frame(u32int frame_addr)
{
    u32int frame = frame_addr / 0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << offset);
}

void clear_frame(u32int frame_addr)
{
    u32int frame = frame_addr / 0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << offset);
}

u32int test_frame(u32int frame_addr)
{
    u32int frame = frame_addr / 0x1000;
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
                if (!(frames[i] & (0x1 << j)))
                {
                    return i * 8 * 4 + j;
                }
            }
        }
    }
}
