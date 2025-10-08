#ifndef VGA_H
#define VGA_H

#include <stdint.h>

typedef enum{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR__LIGHT_BROWN = 14,
    VGA_COLOR__WHITE = 15,
} vga_color;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
void vga_init(void);

void clear_screen(void);
void print_char(char c);
void print_string(const char* str);

void print_hex(uint32_t value);

void move_cursor(void);

void set_cursor_pos(int row, int col);

void set_color(vga_color fg, vga_color bg);

void scroll_up(void);

extern int cursor_row;
extern int cursor_col;

#endif