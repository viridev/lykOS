#include "idt.h"

#include <debug.h>
#include "pic.h"

__attribute__((aligned(0x10)))
idt_entry_t idt_entries[256];
idt_reg_t idt_reg;

void idt_set_entry(uint8_t index, void *handler, uint8_t attributes)
{
    idt_entry_t *entry = &idt_entries[index];

    entry->offset_low = (uint64_t)handler & 0xFFFF;
    entry->kernel_cs = 0x8;
    entry->ist = 0;
    entry->attributes = attributes;
    entry->offset_mid = ((uint64_t)handler >> 16) & 0xFFFF;
    entry->offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

void isr_handler()
{
    // only ints < 32 for now

    __asm__ volatile("cli");
    serial_write("ABC\n");
    // proper error handling for later

    for (;;) __asm__ volatile("hlt");
}

void idt_init()
{
	__asm__ volatile ("cli");

    idt_reg.base = (uintptr_t)&idt_entries;
    idt_reg.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
 
    // Exceptions
    for (uint8_t i = 0; i < 32; i++)
        idt_set_entry(i, &isr_handler, IDT_ENTRY_FLAG_PRESENT | IDT_ENTRY_FLAG_INT);
 
    __asm__ volatile ("lidt %0" : : "m"(idt_reg)); // load the new IDT

    pic_init();

    __asm__ volatile ("sti"); // set the interrupt flag
}