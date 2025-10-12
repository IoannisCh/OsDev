#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// VGA constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

// VGA colors
enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

// Extern cursor position
extern uint8_t cursor_row;
extern uint8_t cursor_col;

// VGA functions
void vga_init();
void vga_clear_screen();
void vga_set_color(enum vga_color fg, enum vga_color bg);
void vga_put_char(char c);
void vga_print(const char *str);
void print_hex(uint32_t value);

// Cursor control
void move_cursor();
void move_cursor_left();
void move_cursor_right();
void move_cursor_up();
void move_cursor_down();

#endif
