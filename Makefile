CFLAGS = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld

OBJ_DIR = build
SRC_DIR = kernel
BOOT_DIR = bootloader

OBJS = $(OBJ_DIR)/kernel.o $(OBJ_DIR)/vga.o $(OBJ_DIR)/paging.o

all: os-image

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/kernel.o: $(SRC_DIR)/kernel.c | $(OBJ_DIR)
	@echo "Compiling kernel.c"
	gcc $(CFLAGS) -c $(SRC_DIR)/kernel.c -o $(OBJ_DIR)/kernel.o

$(OBJ_DIR)/vga.o: $(SRC_DIR)/vga.c | $(OBJ_DIR)
	@echo "Compiling vga.c"
	gcc $(CFLAGS) -c $(SRC_DIR)/vga.c -o $(OBJ_DIR)/vga.o

$(OBJ_DIR)/paging.o: $(SRC_DIR)/paging.c | $(OBJ_DIR)
	@echo "Compiling paging.c"
	gcc $(CFLAGS) -c $(SRC_DIR)/paging.c -o $(OBJ_DIR)/paging.o

kernel.elf: $(OBJS)
	@echo "Linking kernel.elf"
	ld $(LDFLAGS) -o kernel.elf $(OBJS) --oformat elf32-i386

kernel.bin: kernel.elf
	@echo "Generating kernel.bin"
	objcopy -O binary kernel.elf kernel.bin

os-image: $(BOOT_DIR)/boot.asm kernel.bin
	@echo "Building OS image"
	nasm -f bin $(BOOT_DIR)/boot.asm -o boot.bin
	cat boot.bin kernel.bin > os-image

run: os-image
	@echo "Running OS in QEMU"
	qemu-system-i386 -kernel kernel.bin

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) *.bin *.elf os-image
