OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

MEMORY
{
    .text (rx) : ORIGIN = 0x100000, LENGTH = 1M  /* 1MB for code */
    .data (rw) : ORIGIN = 0x110000, LENGTH = 1M  /* 1MB for data */
    .bss (rw) : ORIGIN = 0x120000, LENGTH = 1M   /* 1MB for uninitialized data */
}

SECTIONS
{
    . = ORIGIN(.text);  /* Start at the beginning of .text region */
    .text : {
        *(.multiboot)    /* Bootloader section (if you have one) */
        *(.text)         /* Kernel code */
    } > .text

    . = ORIGIN(.data);  /* Start at the beginning of .data region */
    .data : {
        *(.data)         /* Kernel data */
    } > .data

    . = ORIGIN(.bss);  /* Start at the beginning of .bss region */
    .bss : {
        *(.bss)          /* Uninitialized data */
    } > .bss
}