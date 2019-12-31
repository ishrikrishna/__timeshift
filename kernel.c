#include <screen.h>
#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <io.h>
#include <rtc.h>
#include <kbd.h>
#include <scheduler.h>

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

	sched_queue_add((int)&rtc_wait);
	sched_queue_add((int)&kbd_wait);
	scheduler();
	for(;;);
}

