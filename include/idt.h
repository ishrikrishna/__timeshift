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
extern void keyboard_isr();
extern void attach_isr(int int_num, int isr_addr);
extern void install_idt();
