#include "../include/kernel.h"
#include "../include/paging.h"
#include "../include/keyboard.h"
#include "../include/vga.h"
#include "../include/frame_allocator.h"

void remap_pic() {
    // Remap the PICs to new interrupt vectors
    port_byte_out(0x20, 0x11);  // PIC1 command: ICW1 (initialize)
    port_byte_out(0xA0, 0x11);  // PIC2 command: ICW1 (initialize)
    port_byte_out(0x21, 0x20);  // PIC1 data: ICW2 (start interrupt vector at 32)
    port_byte_out(0xA1, 0x28);  // PIC2 data: ICW2 (start interrupt vector at 40)
    port_byte_out(0x21, 0x04);  // PIC1 data: ICW3 (IRQ1 connected to IRQ2)
    port_byte_out(0xA1, 0x02);  // PIC2 data: ICW3 (IRQ2 is slave to IRQ1)
    port_byte_out(0x21, 0x01);  // PIC1 data: ICW4 (4-bit mode)
    port_byte_out(0xA1, 0x01);  // PIC2 data: ICW4 (4-bit mode)

    // Enable IRQs
    port_byte_out(0x21, 0xFD);  // Unmask IRQ1
    port_byte_out(0xA1, 0xFF);  // Mask all IRQs on PIC2
}

void vga_init() {
    vga_clear_screen();
}

void kernel_main() {
    

    remap_pic();
    vga_init();
    init_paging();
    init_frame_allocator();
    init_keyboard();

    print_string("Welcome to HadOS!\n");

    extern void load_idt();
    load_idt();

    asm volatile("sti");

    while (1) {
        asm volatile("hlt");
    }
    
}
void port_byte_out(uint16_t port, uint8_t data){
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void _start(void) {  // Or int _start(void) - check your ABI
    kernel_main(); // Call your kernel's main function
}



