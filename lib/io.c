#include "io.h"

unsigned char inb(unsigned short port){
	char output = 0;
	asm volatile ("inb %1, %0 \n":"=a"(output):"dN"(port));
	return output;
}

void outb(unsigned short port, char val){
	asm volatile ("outb %1, %0\n"::"dN"(port),"a"(val));
}
