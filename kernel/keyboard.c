#include "../include/keyboard.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

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

static inline uint8_t inb(uint16_t port){
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

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
}

void init_keyboard(){
    register_interrupt_handler(33, keyboard_handler);
}