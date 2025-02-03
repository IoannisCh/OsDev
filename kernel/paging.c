#include "../include/paging.h"
#include "../include/kernel.h"

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
