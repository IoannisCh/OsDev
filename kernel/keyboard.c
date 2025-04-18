#include "../include/keyboard.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/isr.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "../include/io.h"

#define KEYBOARD_DATA_PORT 0x60 

static const char keymap[] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', 
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a','s', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    '\\',
    0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, '*',
    0,
   '*',
    0,
    ' ',
    0,


};

void keyboard_handler(){
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    print_string("Key pressed: ");
    print_char(scancode);

    if(scancode < sizeof(keymap)){
        char c = keymap[scancode];
        if (c) {
            print_char(c);
        }
    }
    print_string("Keyboard interrupt received. \n");

    outb(0x20, 0x20);
}

static void keyboard_callback(registers_t regs){
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    print_string("Key pressed! Scancod: ");
    print_hex(scancode);
    print_string("\n");
}

void init_keyboard(){
    register_interrupt_handler(33, keyboard_handler);

    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);
    outb(0x21, mask);
}
