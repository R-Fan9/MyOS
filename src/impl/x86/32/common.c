#include "common.h"

void* mem_set(void *ptr, s32int val, u32int len){
    u8int *p = ptr;
    while(len > 0){
        *p++ = (u8int)val;
        len--;
    }
    return ptr;
}