#ifndef _RTC_H
#define _RTC_H

struct rtc {
                unsigned short sec;
                unsigned short min;
                unsigned short hour;
                unsigned short week;
                unsigned short day;
                unsigned short month;
                unsigned short year;
                unsigned short century;
} __attribute__ ((packed));

extern void flush_rtc();
extern void display_rtc(int x, int y);
extern void rtc_wait();
extern int rtc_is_set();
extern void rtc_unset();

#endif
