#include "screen.h"
#include "gdt.h"
#include "idt.h"

int _start(){
	char* WelcomeMsg = "Welcome! Kernel loaded successfully.";
	clrscrn();
	puts(10, 6, WelcomeMsg);
	
	asm volatile ("cli");
	// initialize GDT
	install_gdt();
	// initialize IDT
	install_idt();
	asm volatile ("sti");
	
	for(;;);
	return 1;
}

