#include "debug.h"

#include <libc/stdlib.h>
#include <core/serial.h>
#include <core/rtc.h>

#include <stdint.h>
#include <stdarg.h>

void debug_log(const char *str, ...)
{
    va_list list;
    va_start(list, 256);

    char buf[64];
    serial_write("[");
    serial_write(itoa(rtc_hour(), buf, 10));
    serial_write(":");
    serial_write(itoa(rtc_minute(), buf, 10));
    serial_write(":");
    serial_write(itoa(rtc_second(), buf, 10));
    serial_write("] ");

    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '%')
        {
            char buf[64];
            i++;
            if (str[i] == 's')
                serial_write(va_arg(list, const char *));
            else if (str[i] == 'd')
                serial_write(itoa(va_arg(list, int), buf, 10));
            else if (str[i] == 'x')
                serial_write(itoa(va_arg(list, uint64_t), buf, 16));
            else if (str[i] == 'c')
                serial_write_char(va_arg(list, int));
            else if (str[i] == 'l')
            {
                i++;
                if (str[i] == 'l')
                {
                    i++;
                    if (str[i] == 'u')
                        serial_write(ulltoa(va_arg(list, uint64_t), buf, 10));
                    else if (str[i] == 'x')
                        serial_write(ulltoa(va_arg(list, uint64_t), buf, 16));
                }
            }
        }
        else
            serial_write_char(str[i]);
    }
    if (str[i - 1] != '\n')
        serial_write_char('\n');

    va_end(list);
}

void debug_br()
{
    serial_write("--------------------------------------------------\n");    
}