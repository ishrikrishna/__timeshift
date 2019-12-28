#ifndef _KBD_H
#define _KBD_H

extern void kbd_fetch_scancode();
extern char kbd_get_character(char scancode);
extern void kbd_wait();
extern int kbd_is_set();
extern void kbd_unset();

#endif
