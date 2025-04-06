#include "../include/kernel.h"
#include "../include/paging.h"
#include "../include/keyboard.h"
#include "../include/vga.h"
#include "../include/frame_allocator.h"

void vga_init() {
    vga_clear_screen();
}

void kernel_main() {
    vga_init();
    init_paging();
    init_frame_allocator();
    init_keyboard();

    print_string("Welcome to HadOS!\n");

    while (1) {
        asm volatile("hlt");
    }
}

void _start(void) {  // Or int _start(void) - check your ABI
    kernel_main(); // Call your kernel's main function
}

