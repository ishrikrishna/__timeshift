#ifndef _SCREEN_H
#define _SCREEN_H

/** VGA Config */
#define vga_addr 0xb8000
#define vga_char_fgcolor 0xf
#define vga_char_bgcolor 0x0
#define vga_resolution_x 80
#define vga_resolution_y 25

extern void putc(int x, int y, char c);
extern void puts(int x, int y, char* s);
extern void clrscrn();

#endif
