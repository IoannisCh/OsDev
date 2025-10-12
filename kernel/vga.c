#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/io.h"
#include "stdint.h"
#include "stddef.h"

uint8_t cursor_row = 0;
uint8_t cursor_col = 0;
static uint8_t vga_color = 0x07;
static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;

// Internal helper
static uint16_t make_vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void vga_clear_screen() {
    for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = make_vga_entry(' ', vga_color);
    }
    cursor_row = 0;
    cursor_col = 0;
    move_cursor();
}

void vga_set_color(enum vga_color fg, enum vga_color bg) {
    vga_color = fg | bg << 4;
}

void vga_put_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = make_vga_entry(' ', vga_color);
        }
    } else {
        vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = make_vga_entry(c, vga_color);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT) {
        for (int y = 1; y < VGA_HEIGHT; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
            }
        }
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = make_vga_entry(' ', vga_color);
        }
        cursor_row = VGA_HEIGHT - 1;
    }

    move_cursor();
}

void vga_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_put_char(str[i]);
    }
}

void move_cursor() {
    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void move_cursor_left() {
    if (cursor_col > 0) cursor_col--;
    move_cursor();
}

void move_cursor_right() {
    if (cursor_col < VGA_WIDTH - 1) cursor_col++;
    move_cursor();
}

void move_cursor_up() {
    if (cursor_row > 0) cursor_row--;
    move_cursor();
}

void move_cursor_down() {
    if (cursor_row < VGA_HEIGHT - 1) cursor_row++;
    move_cursor();
}

void print_hex(uint32_t value) {
    const char* hex = "0123456789ABCDEF";
    vga_print("0x");
    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (value >> i) & 0xF;
        vga_put_char(hex[nibble]);
    }
}

void vga_init() {
    vga_set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    vga_clear_screen();
}