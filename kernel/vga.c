#include "../include/kernel.h"

unsigned short* VGA_BUFFER = (unsigned short*)0xB8000;

void vga_clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        VGA_BUFFER[i] = (0x0F << 8) | ' ';  // White text on black
    }
}

void vga_print(const char* str) {
    int i = 0;
    while (str[i]) {
        VGA_BUFFER[i] = (0x0F << 8) | str[i];
        i++;
    }
}

void print_char(char c) {
    volatile char *video = (volatile char*)0xB8000;
    *video = c;
}

void print_string(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

void print_hex(uint8_t num){
    const char* hex = "0123456789ABCDEF";
    print_char(hex[(num >> 4) & 0xF]);
    print_char(hex[num & 0xF]);
}

void move_cursor_back(){
    if (cursor_col > 0){
        cursor_col--;
    } else if (cursor_row > 0) {
        cursor_row--;
        curso_col = VGA_WIDTH - 1;
    }

    update_cursor(cursor_row, cursor_col);
}