#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/io.h"

unsigned short* VGA_BUFFER = (unsigned short*)0xB8000;

int cursor_row = 0;
int cursor_col = 0;

void vga_clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_BUFFER[i] = (0x0F << 8) | ' ';  // White text on black
    }

    cursor_row = 0;
    cursor_col = 0;
    update_cursor(cursor_row, cursor_col);
}

void update_cursor(int row, int col){
    unsigned short pos = row * VGA_WIDTH + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void scroll_screen(){
    for (int row = 1; row < VGA_HEIGHT; row++){
        for (int col = 0; col < VGA_WIDTH; col++){
            VGA_BUFFER[(row - 1) * VGA_WIDTH] = VGA_BUFFER[row * VGA_WIDTH + col];
        }
    }

    for (int col = 0; col < VGA_WIDTH; col++){
        VGA_BUFFER[(VGA_HEIGHT -1) * VGA_WIDTH + col] = (0x0F << 8) | ' ';
    }

    if (cursor_row > 0)
        cursor_row--;
}

void print_char(char c) {
    if (c == '\n'){
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\b'){
        move_cursor_back();
    } else {
        VGA_BUFFER[cursor_row * VGA_WIDTH + cursor_col] = (0x0F << 8) | c;
        cursor_col++;
    if (cursor_col >= VGA_WIDTH){
        cursor_col = 0;
        cursor_row++;
    }
    }

    if (cursor_row >= VGA_HEIGHT){
        scroll_screen();
    }

    update_cursor(cursor_row, cursor_col);
}

void print_string(const char* str){
    while (*str)
    {
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
        cursor_col = VGA_WIDTH - 1;
    }

    VGA_BUFFER[cursor_row * VGA_WIDTH + cursor_col] = (0x0F << 8) | ' ';
    update_cursor(cursor_row, cursor_col);
}