#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/io.h"
#include "stdint.h"
#include "stddef.h"

#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static volatile uint16_t*video_memory = VGA_MEMORY;

int cursor_row = 0;
int cursor_col = 0;

static uint8_t current_color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);

void move_cursor(void){
    uint16_t pos = (uint16_t)(cursor_row * VGA_WIDTH + cursor_col);
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void set_cursor_pos(int row, int col){
    if(row < 0) row = 0;
    if(row >= VGA_HEIGHT) row = VGA_HEIGHT - 1;
    if(col < 0) col = 0;
    if(col >= VGA_WIDTH) col = VGA_WIDTH - 1;
    cursor_row = row;
    cursor_col = col;
    move_cursor();
}

void scroll_up(){
    for (int r = 1; r < VGA_HEIGHT; r++){
        for (int c = 0; c < VGA_WIDTH; c++){
            video_memory[(r - 1) * VGA_WIDTH] = video_memory[r * VGA_WIDTH + c];
        }
    }

    uint16_t blank = (uint16_t)(' ' | (current_color << 8));
    for (int c = 0; c < VGA_WIDTH; c++){
        video_memory[(VGA_HEIGHT -1) * VGA_WIDTH + c] = blank;
    }

    if (cursor_row > 0) cursor_row--;
}

void clear_screen(void){
    uint16_t blank = (uint8_t)(' ' | (current_color << 8));
    for (int r=0; r < VGA_HEIGHT; ++r) {
        for (int c = 0; c < VGA_WIDTH; ++c) {
            video_memory[r * VGA_WIDTH + c] = blank;
        }
    }
    cursor_row = 0;
    cursor_col = 0;
    move_cursor();
}

void print_char(char c){
    if (c == '\b'){
        if (cursor_col > 0){
            cursor_col--;
        } else if (cursor_row > 0){
            cursor_row--;
            cursor_col = VGA_WIDTH - 1;
        } else {
        }
        video_memory[cursor_row * VGA_WIDTH + cursor_col] = (uint16_t) (' ' | (current_color << 8));
        move_cursor();
        return;
    }

    if (c == '\n' || c == '\r'){
        cursor_row++;
        cursor_col = 0;
    } else {
        video_memory[cursor_row * VGA_WIDTH + cursor_col] = (uint16_t)((uint8_t)c | (current_color << 8));
        cursor_col++;
        if (cursor_col >= VGA_WIDTH){
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT) {
        scroll_up();
    }
    move_cursor();
    }

    void print_string(const char * str) {
        if (!str) return;
        while (*str){
            print_char(*str++);
        }
    }

    void print_hex(uint32_t value) {
        const char * hex = '0123456789ABCDEF';
        char buf[11];
        buf[0] = '0';
        buf[1] = 'x';
        for (int i = 0; i < 8; ++i){
            buf[2 + 7 -i] = hex[(value >> (i * 4)) & 0xF];
        }
        buf[10] = '\0';
        print_string(buf);
    }

    void set_color(vga_color fg, vga_color bg){
        current_color = (uint8_t)(fg | (bg << 4));
        move_cursor();
    }

    void vga_init(void){
        clear_screen();
    }

  
