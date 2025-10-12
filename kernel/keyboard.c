#include "../include/keyboard.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/isr.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D

static bool shift = false;
static bool caps_lock = false;

static const char keymap[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' ',
    0
};

static const char keymap_shift[] = {
    0, 27, '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,
    'A','S','D','F','G','H','J','K','L',':','@','~',
    0,
    '|','Z','X','C','V','B','N','M','<','>','?',
    0,
    '*',
    0,
    ' ',
    0
};

static void keyboard_callback(registers_t regs) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) { shift = true; return; }
    if (scancode == 0xAA || scancode == 0xB6) { shift = false; return; }

    if (scancode == 0x3A) {
        caps_lock = !caps_lock;
        return;
    }

    if (scancode == ARROW_UP) { move_cursor_up(); return; }
    if (scancode == ARROW_DOWN) { move_cursor_down(); return; }
    if (scancode == ARROW_LEFT) { move_cursor_left(); return; }
    if (scancode == ARROW_RIGHT) { move_cursor_right(); return; }

    if (scancode > sizeof(keymap)) return;

    char c = shift ? keymap_shift[scancode] : keymap[scancode];

    if (caps_lock && c >= 'a' && c <= 'z') c -= 32;
    else if (caps_lock && c >= 'A' && c <= 'Z') c += 32;

    if (c) vga_put_char(c);

    outb(0x20, 0x20);
}

void init_keyboard() {
    register_interrupt_handler(33, keyboard_callback);
}