#pragma once
#include <stdint.h>

typedef struct
{
    uint16_t limit_low;
    uint32_t base_low : 24;
    uint16_t access : 8;
    uint16_t limit_high : 4;
    uint16_t flags : 4;
    uint16_t base_high : 8;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_reg_t;

void gdt_init();