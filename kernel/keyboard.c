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

// Arrow key codes
#define ARROW_UP    0x48
#define ARROW_DOWN  0x50
#define ARROW_LEFT  0x4B
#define ARROW_RIGHT 0x4D

static bool shift = false;
static bool caps_lock = false;

static char key_buffer = 0;
static bool key_ready = false;

static const char keymap[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0,'*',0,' ',0
};

static const char keymap_shift[128] = {
    0, 27, '!','"','£','$','%','^','&','*','(',')','_','+','\b',
    '\t', 'Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','@','~', 0,'|',
    'Z','X','C','V','B','N','M','<','>','?', 0,'*',0,' ',0
};

static void keyboard_callback(registers_t regs) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Handle shift/caps lock
    if (scancode == 0x2A || scancode == 0x36) { shift = true; return; }
    if (scancode == 0xAA || scancode == 0xB6) { shift = false; return; }
    if (scancode == 0x3A) { caps_lock = !caps_lock; return; }

    // Arrow keys
    switch (scancode) {
        case ARROW_UP: move_cursor_up(); return;
        case ARROW_DOWN: move_cursor_down(); return;
        case ARROW_LEFT: move_cursor_left(); return;
        case ARROW_RIGHT: move_cursor_right(); return;
    }

    char c = shift ? keymap_shift[scancode] : keymap[scancode];

    // Caps Lock
    if (caps_lock && c >= 'a' && c <= 'z') c -= 32;

    if (c) {
        key_buffer = c;
        key_ready = true;
    }

    outb(0x20, 0x20); // End of interrupt
}

void keyboard_init() {
    register_interrupt_handler(33, keyboard_callback);
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1); // Unmask IRQ1
    outb(0x21, mask);
}

// Shell call to read a character
char keyboard_read_char() {
    while (!key_ready) asm volatile("hlt");
    key_ready = false;
    return key_buffer;
}