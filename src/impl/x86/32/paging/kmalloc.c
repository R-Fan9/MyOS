unsigned long int placement_address;

unsigned long int kmalloc(unsigned long int sz)
{
    unsigned long int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

unsigned long int kmalloc_ap(unsigned long int sz, int align, unsigned long int *phys)
{
    if (align == 1 && (placement_address && 0xFFFFF000))
    {
        placement_address &= 0xFFFFF000;
        // 0x1000 - 4KB, moves to next page
        placement_address += 0x1000;
    }
    if (phys)
    {
        *phys = placement_address;
    }
    return kmalloc(sz);
}