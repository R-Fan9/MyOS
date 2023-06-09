#include "common.h"

// Write len copies of val into dest.
void memset(void *dest, u8int val, u32int len)
{
    u8int *tmp = (u8int *)dest;
    for (; len != 0; len--)
        *tmp++ = val;
}