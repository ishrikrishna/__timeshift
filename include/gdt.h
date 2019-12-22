/** GDT Config */
//#define gdtr 0x7c2a // 48 bits; 16 => Length, 32 => Start Address
// GDT entries format/structure
struct gdt_entry {
	unsigned short 	limit_low; 		// 0xFFFF
	unsigned short 	base_low;		//0x0
	unsigned char 	base_middle;	//0x0
	unsigned char 	access;  		//10011010b
	unsigned char 	granularity; 	//11001111b
	unsigned char 	base_high;		//0x0
} __attribute__ ((packed));
struct gdtr {
	unsigned short 	limit;
	unsigned int	base_address;
} __attribute__ ((packed));

extern void install_gdt();
