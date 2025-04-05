#include "../include/kernel.h"
#include "../include/paging.h"

void kernel_main() {
    vga_clear_screen();
    vga_print("Welcome to My OS!");
    while (1) {}
}

void _start(void) {  // Or int _start(void) - check your ABI
    kernel_main(); // Call your kernel's main function
}

