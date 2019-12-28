#include <kbd.h>
#include <io.h>
#include <screen.h>

static const char kbd_scancodes[] = {
        ' ', 'e', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', '0', '-', '=', 'd', '\t', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', 'e', 'c', 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
        'r',
        's', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        'c', 'o', 'c', ' ', 'c', 'o', 'l', 'u', 'd', 'r'
        };
/*char test = 'v';
*/
static char kbd_sc = -1;
void kbd_fetch_scancode(){
	kbd_sc = (char)inb(0x60);
}

char kbd_get_character(char scancode){
	return kbd_scancodes[scancode & 0x7f];
}

__attribute__ ((cdecl))
void kbd_wait(unsigned int secs){
	unsigned int wait = sched_ticks + secs;
	while(wait > sched_ticks){
                if(kbd_sc != -1){
                        putc(1,1,kbd_get_character(kbd_sc));
                        kbd_sc = -1;
			break;
                }
		if(sched_ticks > wait){break;}
        }
}
