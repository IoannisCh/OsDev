CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld

all: os-image

kernel.elf: kernel.c vga.c
	gcc $(CFLAGS) -c kernel.c -o kernel.o
	gcc $(CFLAGS) -c vga.c -o vga.o
	ld $(LDFLAGS) -o kernel.elf kernel.o vga.o

os-image: bootloader/boot.asm kernel.elf
	nasm -f bin bootloader/boot.asm -o boot.bin
	cat boot.bin kernel.elf > os-image

iso: os-image
	mkdir -p iso/boot/grub
	cp os-image iso/boot/kernel.bin
	echo 'set timeout=0\nset default=0\nmenuentry "My OS" {\n  multiboot /boot/kernel.bin\n}' > iso/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso iso

run: os-image
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f *.o *.bin *.elf os-image myos.iso
	rm -rf iso
