#include "../include/shell.h"
#include "../include/vga.h"
#include "../include/keyboard.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_INPUT 128

int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *(unsigned char*)a - *(unsigned char*)b;
}

// Clears the screen
void shell_clear() {
    vga_clear_screen();
}

// Prints help
void shell_help() {
    vga_print("Available commands:\n");
    vga_print("help  - Show this message\n");
    vga_print("clear - Clear the screen\n");
}

// Handle a command
void shell_handle_command(char *cmd) {
    if (!strcmp(cmd, "help")) shell_help();
    else if (!strcmp(cmd, "clear")) shell_clear();
    else {
        vga_print("Unknown command: ");
        vga_print(cmd);
        vga_print("\n");
    }
}

// Simple shell loop
void shell_run() {
    char input[128];
    int idx = 0;

    while (1) {
        vga_print("had> ");
        idx = 0;

        // Read input
        while (1) {
            char c = keyboard_read_char();
            if (c == '\n' || c == '\r') {
                input[idx] = '\0';
                vga_print("\n");
                break;
            } else if (c == '\b') { // Backspace
                if (idx > 0) {
                    idx--;
                    move_cursor_left();
                    vga_print(" ");
                    move_cursor_left();
                }
            } else {
                if (idx < 127) {
                    input[idx++] = c;
                    char str[2] = {c, 0};
                    vga_print(str);
                }
            }
        }

        shell_handle_command(input);
    }
}