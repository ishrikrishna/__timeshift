#include <screen.h>
#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <io.h>
#include <rtc.h>
#include <kbd.h>

extern void scheduler();
unsigned int sched_ticks = 0;
unsigned int sched_ticks_reset_interval = 60;
int _start(){
	char* WelcomeMsg = "Welcome! Kernel loaded successfully.";
	clrscrn();
	puts(10, 6, WelcomeMsg);
	
	asm volatile ("cli");
	// initialize GDT
	install_gdt();
	// initialize IDT
	install_idt();
	install_isrs();
	asm volatile ("sti");
	
	scheduler();
	for(;;);
}

void scheduler(){
	rtc_wait();
	kbd_wait(1);
	scheduler();
}
