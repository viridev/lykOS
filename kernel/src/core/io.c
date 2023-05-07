#include "io.h"

void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}

void outl(uint16_t port, uint32_t val)
{
    __asm__ volatile("outl %0, %w1" : : "a" (val), "Nd" (port));
}

uint32_t inl(uint16_t port)
{
    uint32_t data;
    __asm__ volatile("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

void outw(uint16_t port, uint16_t val)
{
    __asm__ volatile("outw %w0, %w1" : : "a" (val), "Nd" (port));
}


uint16_t inw(uint16_t port)
{
    uint16_t data;
    __asm__ volatile("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

void io_wait(void)
{
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}