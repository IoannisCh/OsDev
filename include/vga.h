#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern int cursor_row;
extern int cursor_col;
void vga_init();
void print_char(char c);
void print_string(const char* str);

void clear_screen();

void update_cursor(int row, int col);
void move_cursor_back();

#endif