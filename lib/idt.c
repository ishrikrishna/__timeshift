#include "idt.h"
#include "screen.h"

struct idtr idtr_p;
static struct idt_entry idt[256];

__attribute__ ((naked)) 
void common_isr(){
	asm volatile ("cli\n""pusha");
	char* IntMsg = "Interrupt Received.";
	puts(25,60,IntMsg);
	asm volatile ("mov $0x20, %al\n""out %al, $0x20\n""popa\n""iret");
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
	
	asm volatile ("lidt %0":: [idtr_p]"m"(idtr_p));;
}

struct idt_entry* get_idt_entry(int int_num){
	return (struct idt_entry *)((int)idt + (int_num << 3));
}
