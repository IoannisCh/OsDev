#include "../include/frame_allocator.h"
#include "../include/kernel.h"

#define FRAME_SIZE 4096
#define BITMAP_SIZE 32768

static uint32_t frame_bitmap[BITMAP_SIZE / 32];

#define INDEX_FROM_BIT(a) (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

void set_frame(uint32_t addr){
    uint32_t frame = addr / FRAME_SIZE;
    frame_bitmap[INDEX_FROM_BIT(frame)] |= (1 << OFFSET_FROM_BIT(frame));
}

void clear_frame(uint32_t addr){
    uint32_t frame = addr / FRAME_SIZE;
    frame_bitmap[INDEX_FROM_BIT(frame)] &= ~(1 << OFFSET_FROM_BIT(frame));
}

int test_frame(uint32_t addr){
    uint32_t frame = addr / FRAME_SIZE;
    return (frame_bitmap[INDEX_FROM_BIT(frame)] & (1 << OFFSET_FROM_BIT(frame)));
}

uint32_t first_frame(){
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        if (frame_bitmap[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                uint32_t frame = i * 32 + j;
                if (!(frame_bitmap[i] & (1 << j))) {
                    return frame * FRAME_SIZE;
                }
            }
        }
    }
    return (uint32_t)-1;
}

uint32_t alloc_frame(){
    uint32_t frame = first_frame();
    if (frame == (uint32_t)-1) return 0;
        set_frame(frame * FRAME_SIZE);
        return frame * FRAME_SIZE;
    
    return 0;
}


void free_frame(uint32_t addr){
    clear_frame(addr);
}


