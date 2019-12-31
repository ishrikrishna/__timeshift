#include <rtc.h>
#include <io.h>
#include <screen.h>

struct rtc ctime;
int ist_tz = 530;
int rtc_available = 0;
int time_elapsed = 0;

void itoa(unsigned int val, char* out, int len){
	unsigned int idx = len - 1;
	unsigned int divisor = val;
	while(divisor != 0){
		out[idx] = divisor%10;
		out[idx] += 0x30;
		divisor /= 10;
		idx--;
	}
}

void format_rtc_val(short val, short* out){
	*out = ((val >> 4 & 0x000f) | (val << 8 & 0x0f00)) + 0x3030;
}

void flush_rtc(){
	time_elapsed++;
	if(!rtc_available && time_elapsed >= 1024){
		time_elapsed = 0;
		outb(0x70, 0x00);
		ctime.sec = inb(0x71);
		outb(0x70, 0x02);
        	ctime.min = inb(0x71);
        	outb(0x70, 0x04);
        	ctime.hour = inb(0x71);
		outb(0x70, 0x06);
		ctime.week = inb(0x71);
        	outb(0x70, 0x07);
        	ctime.day = inb(0x71);
        	outb(0x70, 0x08);
        	ctime.month = inb(0x71);
        	outb(0x70, 0x09);
        	ctime.year = inb(0x71);
		outb(0x70, 0x32);
       		ctime.century = inb(0x71);
		rtc_available = 1;
	}
}

void display_rtc(int x, int y){
	char *ptime = "00:00:00 00/00/0000";
	
	format_rtc_val(ctime.sec, (short *)(ptime+6));
	short *min = (short *)(ptime+3);
	format_rtc_val(ctime.min, min);
	short *hour = (short *)(ptime);
	format_rtc_val(ctime.hour, hour);
	short *day = (short *)(ptime+9);
	format_rtc_val(ctime.day, day);
	short *month = (short *)(ptime+12);
	format_rtc_val(ctime.month, month);
	format_rtc_val(ctime.century, (short *)(ptime+15));
	format_rtc_val(ctime.year, (short *)(ptime+17));

	puts(x, y, ptime);
}

int rtc_is_set(){
	return rtc_available;
}
void rtc_clear(){
	rtc_available = 0;
}
void rtc_wait(){
	if(rtc_is_set()){
		display_rtc(60, 1);	
		rtc_clear();
	}
}
