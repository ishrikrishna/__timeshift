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
extern unsigned int sched_ticks;
extern unsigned int sched_ticks_reset_interval;

extern void update_sched_ticks();
extern void flush_rtc();
extern void display_rtc(int x, int y);
extern void rtc_wait();

#endif
