#include "screen.h"
/*
 * VGA Screen routines
 */ 
void putc(int x, int y, char c) {
	short color = vga_char_fgcolor | vga_char_bgcolor << 4;
	*(short *)(vga_addr + ((--x + --y * vga_resolution_x) << 1)) = c | color << 8;
}
void puts(int x, int y, char* s) {
	do{
		if(x>=vga_resolution_x){ y++; x=0; }
		putc(++x, y, *s );
		s++;
	}while(*s != 0);
}
void clrscrn(){
	int x=0, y=0;
	while(y<=25){
		while(x<=80){
			*(short *)(vga_addr + ((x + y * vga_resolution_x) << 1)) = 0x0820;
			x++;
		}
		x=1;
		y++;
	}
}

