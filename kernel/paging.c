#include "../include/paging.h"
#include "../include/kernel.h"
#include "../include/frame_allocator.h"
#include "stddef.h"

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

void map_page(uint32_t virtual_addr, uint32_t flags){
    uint32_t pd_index = virtual_addr >> 22;
    uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;

    if (!(page_directory[pd_index] & 1)){
        uint32_t * new_page_table = (uint32_t*)alloc_frame();
        if (new_page_table == NULL){
            panic("Failed to allocate new page table");
        }

        for (int i = 0; i < 1024; i++){
            new_page_table[i] = 0;
        }

        page_directory[pd_index] = ((uint32_t)new_page_table) | flags | 2; 
    }

    uint32_t * page_table = (uint32_t*)((page_directory[pd_index] & 0xFFFFF000));

    uint32_t frame = alloc_frame();
    if (frame == 0){
        panic("Failed to allocate new frame");
    }

    page_table[pt_index] = frame | flags | 1;
}
