#include "kernel.h"

void kernel_main() {
    vga_clear_screen();
    vga_print("Welcome to My OS!");
    while (1) {}
}
