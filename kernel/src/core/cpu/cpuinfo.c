#include "cpuinfo.h"

#include <cpuid.h>

static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

void cpuinfo_detect()
{
    debug_br();

    uint32_t unused;

    // Get CPU vendor ID.
    cpuid(0, &unused, (uint32_t *)(cpu_vendor_id + 0), (uint32_t *)(cpu_vendor_id + 8), (uint32_t *)(cpu_vendor_id + 4));
    cpu_vendor_id[12] = '\0';
    debug_log("CPU vendor: %s", cpu_vendor_id);

    // Get CPU full name.
    cpuid(0x80000002, (uint32_t *)(cpu_name +  0), (uint32_t *)(cpu_name +  4), (uint32_t *)(cpu_name +  8), (uint32_t *)(cpu_name + 12));
    cpuid(0x80000003, (uint32_t *)(cpu_name + 16), (uint32_t *)(cpu_name + 20), (uint32_t *)(cpu_name + 24), (uint32_t *)(cpu_name + 28));
    cpuid(0x80000004, (uint32_t *)(cpu_name + 32), (uint32_t *)(cpu_name + 36), (uint32_t *)(cpu_name + 40), (uint32_t *)(cpu_name + 44));
    debug_log("CPU name: %s", cpu_name);

    debug_br();
}
