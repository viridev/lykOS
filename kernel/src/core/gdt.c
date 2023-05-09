#include "gdt.h"

#include <lib/debug.h>

// gdt_entry_t gdt_entries[7];
gdt_reg_t gdt_reg;

// void gdt_set_entry(int index, uint32_t base, uint16_t limit, uint8_t access, uint8_t gran)
// {
//     gdt_entry_t *entry = &gdt_entries[index];

//     entry->base_low = base & 0xFFFF;
//     entry->base_mid = (base >> 16) & 0xFF;
//     entry->base_high = (base >> 24) & 0xFF;

//     entry->limit = limit;
//     entry->granularity = gran;
// }

uint64_t gdt_data[9] = {
    0x0000000000000000,

    0x00009a000000ffff, // 16-bit code
    0x000093000000ffff, // 16-bit data

    0x00cf9a000000ffff, // 32-bit code
    0x00cf93000000ffff, // 32-bit data

    0x00af9b000000ffff, // 64-bit code
    0x00af93000000ffff, // 64-bit data

    0x00affb000000ffff, // usermode 64-bit code
    0x00aff3000000ffff, // usermode 64-bit data
};

extern gdt_flush(uint64_t gdt_reg_addr);

void gdt_init()
{
    gdt_reg.base = (uint64_t)&gdt_data;
    gdt_reg.limit = (sizeof(gdt_entry_t) * 9) - 1;

    __asm__ volatile("lgdt %0" : : "m"(gdt_reg) : "memory");

    debug_log("GDT initialized.");
}