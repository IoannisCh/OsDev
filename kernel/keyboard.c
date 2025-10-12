#include "../include/keyboard.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/isr.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static bool shift_pressed = false;
static bool caps_lock = false;

#define ARROW_UP    0x48
#define ARROW_LEFT  0x4B
#define ARROW_RIGHT 0x4D
#define ARROW_DOWN  0x50



static const char keymap[128] = {
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

static const char keymap_shift[128] = {
    0, 27, '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '~',
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0,
};



static void keyboard_callback(registers_t regs){
    (void)regs;

    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

   if (scancode & 0x80){
    scancode &= 0x7F;
        if (scancode == 42 || scancode == 54) shift_pressed = false;
        return;
   }

   if (scancode == 42 || scancode == 54) {
    shift_pressed = true;
    return;
   }

   if (scancode == 58){
    caps_lock = !caps_lock;
    return;
   }

   switch(scancode){
    case ARROW_UP: move_cursor_up(); return;
    case ARROW_DOWN: move_cursor_down(); return;
    case ARROW_LEFT: move_cursor_left(); return;
    case ARROW_RIGHT: move_cursor_right(); return;
    default: break;
   }

   char c = 0;
   if (shift_pressed)
    c = keymap_shift[scancode];
   else
    c = keymap[scancode];


    if (caps_lock && c >= 'a' && c <= 'Z'){
        c = c - 'a' + 'A';
    } else if (caps_lock && c >= 'A' && c <= 'Z'){
        c = c - 'A' + 'a';
    }

    if (c)
        print_char(c);

   
}

void init_keyboard(){
    register_interrupt_handler(33, keyboard_callback);

    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);
    outb(0x21, mask);

    print_string("Keyboard (UK layout) initialized. \n");
}
