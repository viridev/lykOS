#pragma once
#include <stdint.h>

char cpu_vendor_id[13];
char cpu_name[49];

void cpuinfo_detect();