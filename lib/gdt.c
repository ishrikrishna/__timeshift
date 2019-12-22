#include "gdt.h"

struct gdtr gdtr_p;
static struct gdt_entry gdt[16];

void install_gdt(){
	struct gdt_entry entry;
	
	entry.limit_low = 0x0;
	entry.base_low = 0x0;
	entry.base_middle = 0x0;
	entry.access = 0x0;
	entry.granularity = 0x0;
	entry.base_high = 0x0;
	gdt[0] = entry;
	
	entry.limit_low = 0xffff;
	entry.base_low = 0x0;
	entry.base_middle = 0x0;
	entry.access = 0x9a;
	entry.granularity = 0xcf;
	entry.base_high = 0x0;
	
	gdt[1] = entry;
	entry.access = 0x92;
	gdt[2] = entry;
	entry.access = 0xfa;
	gdt[3] = entry;
	entry.access = 0xf2;
	gdt[4] = entry;
	
	gdtr_p.limit = (short)40;
	gdtr_p.base_address = (int)gdt;
	asm volatile ("lgdt %0":: [gdtr_p]"m"(gdtr_p));
}
