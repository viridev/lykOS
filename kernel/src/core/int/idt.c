#include "idt.h"
#include "pic.h"

#include <lib/debug.h>
#include <libc/stdlib.h>

__attribute__((aligned(0x10)))
idt_entry_t idt_entries[256];
idt_reg_t idt_reg;

void idt_set_entry(uint8_t index, void *handler, uint8_t attributes)
{
    idt_entry_t *entry = &idt_entries[index];

    entry->offset_low = (uint64_t)handler & 0xFFFF;
    entry->kernel_cs = 0x28;
    entry->ist = 0;
    entry->attributes = attributes;
    entry->offset_mid = ((uint64_t)handler >> 16) & 0xFFFF;
    entry->offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

const char *error_msg[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Detected overflow",
    "Out-of-bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void int_handler(int_debug_regs *regs)
{
    if (regs->int_no < 32) // isr
    {
        __asm__ volatile("cli");

        debug_log("CPU exception: %llu - %s", regs->int_no, error_msg[regs->int_no]);
        debug_log("System halted.");

        for (;;)
            __asm__ volatile("hlt");
    }
    else // irq
    {
        pci_eoi(regs->int_no - 32);
    }       
}

void idt_init()
{
    __asm__ volatile("cli");

    idt_reg.base = (uint64_t)&idt_entries;
    idt_reg.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);

    {
        // uint8_t attr = (IDT_ENTRY_FLAG_PRESENT | IDT_ENTRY_FLAG_INT);
        uint8_t attr = 0x8E;

        idt_set_entry(0, &int_stub_0, attr);
        idt_set_entry(1, &int_stub_1, attr);
        idt_set_entry(2, &int_stub_2, attr);
        idt_set_entry(3, &int_stub_3, attr);
        idt_set_entry(4, &int_stub_4, attr);
        idt_set_entry(5, &int_stub_5, attr);
        idt_set_entry(6, &int_stub_6, attr);
        idt_set_entry(7, &int_stub_7, attr);
        idt_set_entry(8, &int_stub_8, attr);
        idt_set_entry(9, &int_stub_9, attr);
        idt_set_entry(10, &int_stub_10, attr);
        idt_set_entry(11, &int_stub_11, attr);
        idt_set_entry(12, &int_stub_12, attr);
        idt_set_entry(13, &int_stub_13, attr);
        idt_set_entry(14, &int_stub_14, attr);
        idt_set_entry(15, &int_stub_15, attr);
        idt_set_entry(16, &int_stub_16, attr);
        idt_set_entry(17, &int_stub_17, attr);
        idt_set_entry(18, &int_stub_18, attr);
        idt_set_entry(19, &int_stub_19, attr);
        idt_set_entry(20, &int_stub_20, attr);
        idt_set_entry(21, &int_stub_21, attr);
        idt_set_entry(22, &int_stub_22, attr);
        idt_set_entry(23, &int_stub_23, attr);
        idt_set_entry(24, &int_stub_24, attr);
        idt_set_entry(25, &int_stub_25, attr);
        idt_set_entry(26, &int_stub_26, attr);
        idt_set_entry(27, &int_stub_27, attr);
        idt_set_entry(28, &int_stub_28, attr);
        idt_set_entry(29, &int_stub_29, attr);
        idt_set_entry(30, &int_stub_30, attr);
        idt_set_entry(31, &int_stub_31, attr);
        idt_set_entry(32, &int_stub_32, attr);
        idt_set_entry(33, &int_stub_33, attr);
        idt_set_entry(34, &int_stub_34, attr);
        idt_set_entry(35, &int_stub_35, attr);
        idt_set_entry(36, &int_stub_36, attr);
        idt_set_entry(37, &int_stub_37, attr);
        idt_set_entry(38, &int_stub_38, attr);
        idt_set_entry(39, &int_stub_39, attr);
        idt_set_entry(40, &int_stub_40, attr);
        idt_set_entry(41, &int_stub_41, attr);
        idt_set_entry(42, &int_stub_42, attr);
        idt_set_entry(43, &int_stub_43, attr);
        idt_set_entry(44, &int_stub_44, attr);
        idt_set_entry(45, &int_stub_45, attr);
        idt_set_entry(46, &int_stub_46, attr);
        idt_set_entry(47, &int_stub_47, attr);
    }

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idt_reg)); // load the new IDT

    pic_init(); // enable the PIC before setting the interrupt flag

    __asm__ volatile("sti"); // set the interrupt flag

    debug_log("IDT initialized.");
}