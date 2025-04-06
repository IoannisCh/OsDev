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

