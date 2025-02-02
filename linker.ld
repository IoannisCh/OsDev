ENTRY(_start)

SECTIONS
{
    . = 1M;  /* Load kernel at 1MB */
    
    .text : {
        *(.multiboot)   /* Bootloader section */
        *(.text)        /* Kernel code */
    }

    .data : {
        *(.data)        /* Kernel data */
    }

    .bss : {
        *(.bss)         /* Uninitialized data */
    }
}
