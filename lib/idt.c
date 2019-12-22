#include "idt.h"
#include "screen.h"

struct idtr idtr_p;
static struct idt_entry idt[256];

__attribute__ ((naked)) 
void common_isr(){
	asm volatile ("cli\n""pusha");
	char* IntMsg = "Interrupt Received.";
	puts(6,20,IntMsg);
	asm volatile ("mov $0x20, %al\n""out %al, $0x20\n""popa\n""iret");
}

__attribute__ ((naked)) 
void keyboard_isr(){
	asm volatile ("cli\n""pusha");
	char* IntMsg = "Keyboard Interrupt.";
	puts(6,15,IntMsg);
	asm volatile ("mov $0x20, %al\n""out %al, $0x20\n""popa\n""iret");
}

void attach_isr(int int_num, int isr_addr){
	struct idt_entry *curr_entry = (struct idt_entry *)((int)idt + (int_num << 3));
	curr_entry->base_low = (isr_addr & 0x0000ffff);
	curr_entry->base_high = (isr_addr >> 16) & 0x0000ffff;
}

void install_idt(){
	struct idt_entry entry;
	entry.base_low = (int)common_isr & 0x0000ffff;
	entry.selector = 0x8;
	entry.reserved = 0x0;
	entry.type = 0x8e;
	entry.base_high = ((int)common_isr >> 16) & 0x0000ffff;
	for(int i = 0; i<256; i++){
		idt[i] = entry;
	}
	idtr_p.limit = (short)sizeof(idt);
	idtr_p.base_address = (int)idt;
	
	//Attach ISR
	attach_isr(33, (int)keyboard_isr);
	asm volatile ("lidt %0":: [idtr_p]"m"(idtr_p));;
}
