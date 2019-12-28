#include <idt.h>
#include <isrs.h>
#include <rtc.h>
#include <io.h>
#include <screen.h>
#include <kbd.h>

//extern int kbd_sc;

__attribute__ ((cdecl, naked))
void keyboard_isr(){
	asm volatile ("cli \n\t" "pusha \n\t");
	kbd_fetch_scancode();
	asm volatile (
			"mov $0x20, %al \n\t"
			"out %al, $0x20 \n\t"
			"popa \n\t"
			"iret \n\t"
			);
}

__attribute__ ((cdecl, naked))
void timer_isr(){
        asm volatile ("cli \n\t" "pusha \n\t");
        outb(0x70, 0x0c);
        inb(0x71);
        outb(0x20, 0x20);
        //outb(0xa0, 0xa0);
        asm volatile ("popa \n\t" "iret \n\t");
}

__attribute__ ((cdecl, naked))
void rtc_isr(){
	asm volatile ("cli \n\t" "pusha \n\t");
	flush_rtc();
	update_sched_ticks();
	outb(0x70, 0x0c);
	inb(0x71);
	outb(0xa0, 0xa0);
	outb(0x20, 0x20);
	asm volatile ( "popa \n\t" "iret \n\t");
}

void attach_isr(int int_num, int isr_addr){
        struct idt_entry *curr_entry = get_idt_entry(int_num);
        curr_entry->base_low = (isr_addr & 0x0000ffff);
        curr_entry->base_high = (isr_addr >> 16) & 0x0000ffff;
}

void install_isrs(){
        attach_isr(32, (int)&timer_isr);
	attach_isr(33, (int)&keyboard_isr);
       	attach_isr(40, (int)&rtc_isr);
}

