#include "idt.h"
#include "pic.h"

#include <debug.h>
#include <lib/stdlib.h>

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

void isr_handler(int_debug_regs *regs)
{
    __asm__ volatile("cli");

    char buf[10];
    serial_write(itoa((int)regs->int_no, buf, 10));

    for (;;)
        __asm__ volatile("hlt");
}

void idt_init()
{
    __asm__ volatile("cli");

    idt_reg.base = (uint64_t)&idt_entries;
    idt_reg.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    {
        uint8_t attr = IDT_ENTRY_FLAG_PRESENT | IDT_ENTRY_FLAG_INT;

        idt_set_entry(0, (uint64_t)&isr_stub_0, attr);
        idt_set_entry(1, (uint64_t)&isr_stub_1, attr);
        idt_set_entry(2, (uint64_t)&isr_stub_2, attr);
        idt_set_entry(3, (uint64_t)&isr_stub_3, attr);
        idt_set_entry(4, (uint64_t)&isr_stub_4, attr);
        idt_set_entry(5, (uint64_t)&isr_stub_5, attr);
        idt_set_entry(6, (uint64_t)&isr_stub_6, attr);
        idt_set_entry(7, (uint64_t)&isr_stub_7, attr);
        idt_set_entry(8, (uint64_t)&isr_stub_8, attr);
        idt_set_entry(9, (uint64_t)&isr_stub_9, attr);
        idt_set_entry(10, (uint64_t)&isr_stub_10, attr);
        idt_set_entry(11, (uint64_t)&isr_stub_11, attr);
        idt_set_entry(12, (uint64_t)&isr_stub_12, attr);
        idt_set_entry(13, (uint64_t)&isr_stub_13, attr);
        idt_set_entry(14, (uint64_t)&isr_stub_14, attr);
        idt_set_entry(15, (uint64_t)&isr_stub_15, attr);
        idt_set_entry(16, (uint64_t)&isr_stub_16, attr);
        idt_set_entry(17, (uint64_t)&isr_stub_17, attr);
        idt_set_entry(18, (uint64_t)&isr_stub_18, attr);
        idt_set_entry(19, (uint64_t)&isr_stub_19, attr);
        idt_set_entry(20, (uint64_t)&isr_stub_20, attr);
        idt_set_entry(21, (uint64_t)&isr_stub_21, attr);
        idt_set_entry(22, (uint64_t)&isr_stub_22, attr);
        idt_set_entry(23, (uint64_t)&isr_stub_23, attr);
        idt_set_entry(24, (uint64_t)&isr_stub_24, attr);
        idt_set_entry(25, (uint64_t)&isr_stub_25, attr);
        idt_set_entry(26, (uint64_t)&isr_stub_26, attr);
        idt_set_entry(27, (uint64_t)&isr_stub_27, attr);
        idt_set_entry(28, (uint64_t)&isr_stub_28, attr);
        idt_set_entry(29, (uint64_t)&isr_stub_29, attr);
        idt_set_entry(30, (uint64_t)&isr_stub_30, attr);
        idt_set_entry(31, (uint64_t)&isr_stub_31, attr);
    }

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idt_reg)); // load the new IDT

    pic_init(); // enable the PIC before setting the interrupt flag

    __asm__ volatile("sti"); // set the interrupt flag
}