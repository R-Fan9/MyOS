#include "common.h"

#ifndef kHEAP_H
#define KHEAP_H

extern u32int placement_addr;

u32int kmalloc_int(u32int sz, int align, u32int *phys);
u32int kmalloc_a(u32int sz);
u32int kmalloc_ap(u32int sz, u32int *phys);
u32int kmalloc(u32int sz);

#endif