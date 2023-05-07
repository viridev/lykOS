#pragma once
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

#define IDT_ENTRY_FLAG_PRESENT 1 << 8
#define IDT_ENTRY_FLAG_INT 0b1110
#define IDT_ENTRY_FLAG_TRAP 0b1111

typedef struct
{
	uint16_t offset_low;
	uint16_t kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	 ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t  attributes;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_reg_t;

void idt_init();