#include "rtc.h"
#include <core/io.h>

uint8_t bcd_to_bin(uint8_t value)
{
    return (value >> 4) * 10 + (value & 0xF);
}

void rtc_read_reg(int reg)
{
    outb(RTC_ADDR, reg);
    return inb(RTC_DATA);
}

void rtc_set_reg(uint16_t reg, uint8_t val)
{
    outb(RTC_ADDR, reg);
    outb(RTC_DATA, val);
}

int rtc_year()
{
    outb(0x70, RTC_REG_YEAR);
    return bcd_to_bin(inb(0x71));
}

int rtc_month()
{
    outb(0x70, RTC_REG_MONTH);
    return bcd_to_bin(inb(0x71));
}

int rtc_day()
{
    outb(0x70, RTC_REG_DAY);
    return bcd_to_bin(inb(0x71));
}

int rtc_day_of_the_weak()
{
    outb(0x70, RTC_REG_WEEK_DAY);
    return bcd_to_bin(inb(0x71));
}

int rtc_hour()
{
    outb(0x70, RTC_REG_HOUR);
    return bcd_to_bin(inb(0x71));
}

int rtc_minute()
{
    outb(0x70, RTC_REG_MIN);
    return bcd_to_bin(inb(0x71));
}

int rtc_second()
{
    outb(0x70, RTC_REG_SEC);
    return bcd_to_bin(inb(0x71));
}
