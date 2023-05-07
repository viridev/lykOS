#include "gdt.h"

gdt_entry_t gdt_entries[5];
gdt_reg_t gdt_reg;

void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt_entry_t *entry = &gdt_entries[index];

    entry->base_low = base & 0xFFFFFFF;
    entry->base_high = base >> 24;

    entry->limit_low = base & 0xFFFFF;
    entry->limit_high = base >> 16;

    entry->access = access;
    entry->flags = flags;
}

void gdt_init()
{
    gdt_reg.base = &gdt_entries;
    gdt_reg.limit = sizeof(gdt_entry_t) * 5 - 1;

    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xC);
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xA);
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xC);

    __asm__ volatile("lgdt %0" : : "m"(gdt_reg));
}