#ifndef _IDT_H
#define _IDT_H

/** IDT Config */
//#define idtr 0x7e30 // 48 bits; 16 => Length, 32 => Start Address
// IDT entries format/structure

struct idt_entry {
	unsigned short 	base_low;
	unsigned short 	selector;
	unsigned char 	reserved;  
	unsigned char 	type; 
	unsigned short 	base_high;
} __attribute__ ((packed));
struct idtr {
	unsigned short 	limit;
	unsigned int	base_address;
} __attribute__ ((packed));

extern void common_isr();
extern struct idt_entry* get_idt_entry(int int_num);
extern void install_idt();

#endif
