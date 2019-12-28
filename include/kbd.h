#ifndef _KBD_H
#define _KBD_H

extern unsigned int sched_ticks;
extern void kbd_fetch_scancode();
extern char kbd_get_character(char scancode);
extern void kbd_wait();

#endif
