#include <stddef.h>

void panic(const char* message) {
    (void)message;
    while (1) { __asm__ volatile("hlt"); }
}
