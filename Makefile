CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -m elf_i386 -T linker.ld

OBJ_DIR = build
SRC_DIR = kernel
BOOT_DIR = bootloader
ASM_DIR = asm

QEMU = /usr/bin/qemu-system-x86_64  # Or the correct path from 'which qemu-system-i386'

OBJS = $(OBJ_DIR)/kernel.o $(OBJ_DIR)/vga.o $(OBJ_DIR)/paging.o $(OBJ_DIR)/paging_asm.o

all: os-image

$(OBJ_DIR):
	@echo "Creating build directory..."
	mkdir -p $(OBJ_DIR) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..." # TAB before @echo
	gcc $(CFLAGS) -c $< -o $@ # TAB before gcc

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.asm | $(OBJ_DIR)
	@echo "Assembling $<..." # TAB before @echo
	nasm -f elf32 $(ASM_DIR)/%.asm -o $@ 

$(OBJ_DIR)/paging_asm.o: $(ASM_DIR)/paging.asm | $(OBJ_DIR)  # Explicit rule
	@echo "Assembling $<..."
	nasm -f elf32 $< -o $@	
kernel.elf: $(OBJS)
	@echo "Linking kernel.elf..." # TAB before @echo
	ld $(LDFLAGS) -o kernel.elf $(OBJS) --oformat elf32-i386 # TAB before ld

kernel.bin: kernel.elf
	@echo "Generating kernel.bin..." # TAB before @echo
	objcopy -O binary kernel.elf kernel.bin # TAB before objcopy

os-image: $(BOOT_DIR)/boot.asm kernel.bin
	@echo "Assembling bootloader..." # TAB before @echo
	nasm -f bin $(BOOT_DIR)/boot.asm -o boot.bin # TAB before nasm
	@echo "Building OS image..." # TAB before @echo
	cat boot.bin kernel.bin > os-image

run: os-image
	@echo "Running OS in QEMU..." # TAB before @echo
	$(QEMU) -kernel os-image

clean:
	@echo "Cleaning up..." # TAB before @echo
	rm -rf $(OBJ_DIR) *.bin *.elf os-image # TAB before rm