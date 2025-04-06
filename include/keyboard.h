#ifndef KEYBOARD_H
#define KEYBOARD_H

void init_keyboard();

void register_interrupt_handler(int n, void (*handler)(void));

#endif