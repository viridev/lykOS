#pragma once

#define RTC_ADDR 0x70
#define RTC_DATA 0x71

#define RTC_REG_SEC                         0x00
#define RTC_REG_MIN                         0x02
#define RTC_REG_HOUR                        0x04
#define RTC_REG_WEEK_DAY                    0x06
#define RTC_REG_DAY                         0x07
#define RTC_REG_MONTH                       0x08
#define RTC_REG_YEAR                        0x09
#define RTC_REG_A                           0x0a
#define RTC_REG_B                           0x0b
#define RTC_REG_C                           0x0c
#define RTC_REG_D                           0x0d

int rtc_year();
int rtc_month();
int rtc_day();
int rtc_day_of_the_weak();
int rtc_hour();
int rtc_minute();
int rtc_second();