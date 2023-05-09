#pragma once
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

#define IDT_ENTRY_FLAG_PRESENT 1 << 7
#define IDT_ENTRY_FLAG_INT 0b1110
#define IDT_ENTRY_FLAG_TRAP 0b1111

#include <limine.h>
struct limine_framebuffer *framebuffer;

typedef struct
{
	uint16_t offset_low;
	uint16_t kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the isr
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

typedef struct
{
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t int_no;
	uint64_t err;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) int_debug_regs;

void idt_init();

extern void int_stub_0();
extern void int_stub_1();
extern void int_stub_2();
extern void int_stub_3();
extern void int_stub_4();
extern void int_stub_5();
extern void int_stub_6();
extern void int_stub_7();
extern void int_stub_8();
extern void int_stub_9();
extern void int_stub_10();
extern void int_stub_11();
extern void int_stub_12();
extern void int_stub_13();
extern void int_stub_14();
extern void int_stub_15();
extern void int_stub_16();
extern void int_stub_17();
extern void int_stub_18();
extern void int_stub_19();
extern void int_stub_20();
extern void int_stub_21();
extern void int_stub_22();
extern void int_stub_23();
extern void int_stub_24();
extern void int_stub_25();
extern void int_stub_26();
extern void int_stub_27();
extern void int_stub_28();
extern void int_stub_29();
extern void int_stub_30();
extern void int_stub_31();
extern void int_stub_32();
extern void int_stub_33();
extern void int_stub_34();
extern void int_stub_35();
extern void int_stub_36();
extern void int_stub_37();
extern void int_stub_38();
extern void int_stub_39();
extern void int_stub_40();
extern void int_stub_41();
extern void int_stub_42();
extern void int_stub_43();
extern void int_stub_44();
extern void int_stub_45();
extern void int_stub_46();
extern void int_stub_47();