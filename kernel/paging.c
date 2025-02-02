#include "../include/paging.h"
#include "../include/kernel.h"
#include <stdint.h>

unsigned int page_directory[1024] __attribute__((aligned(4096)));
unsigned int first_page_table[1024] __attribute__((aligned(4096)));

void init_paging() {
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3; // Present, RW enabled
    }
    page_directory[0] = ((uintptr_t)first_page_table) | 3;
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0;
    }

    load_page_directory(page_directory);
    enable_paging();
}

void load_page_directory(uint32_t* dir) {
    asm volatile(
        "movl %0, %%eax\n" // Move dir into EAX first
        "movl %%eax, %%cr3" // Then move EAX to CR3
        :
        : "r"(dir)
        : "eax", "memory"
    );
}

void enable_paging() {
    asm volatile(
        "movl %%cr0, %%eax\n"  // Move CR0 to EAX
        "orl $0x80000000, %%eax\n"  // Set paging bit
        "movl %%eax, %%cr0"  // Move EAX back to CR0
        :
        :
        : "eax"
    );
}
