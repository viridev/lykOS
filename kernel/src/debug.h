#pragma once
#include <stdint.h>

#define PORT 0x3f8  // COM1

void serial_write_char(char a);
void serial_write(const char *str);
void serial_init();