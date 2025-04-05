CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld

OBJ_DIR = build
SRC_DIR = kernel
BOOT_DIR = bootloader
ASM_DIR = asm

# Add frame_allocator.o to object list
OBJS = $(OBJ_DIR)/kernel.o \
       $(OBJ_DIR)/vga.o \
       $(OBJ_DIR)/paging.o \
       $(OBJ_DIR)/paging_asm.o \
       $(OBJ_DIR)/frame_allocator.o\
	   $(OBJ_DIR)/panic.o

all: os-image

# Ensure build dir exists
$(OBJ_DIR):
	@echo "Creating build directory..."
	mkdir -p $(OBJ_DIR)

# Build each C source file
$(OBJ_DIR)/kernel.o: $(SRC_DIR)/kernel.c | $(OBJ_DIR)
	@echo "Compiling kernel.c..."
	gcc $(CFLAGS) -c $(SRC_DIR)/kernel.c -o $@

$(OBJ_DIR)/vga.o: $(SRC_DIR)/vga.c | $(OBJ_DIR)
	@echo "Compiling vga.c..."
	gcc $(CFLAGS) -c $(SRC_DIR)/vga.c -o $@

$(OBJ_DIR)/paging.o: $(SRC_DIR)/paging.c | $(OBJ_DIR)
	@echo "Compiling paging.c..."
	gcc $(CFLAGS) -c $(SRC_DIR)/paging.c -o $@

$(OBJ_DIR)/frame_allocator.o: $(SRC_DIR)/frame_allocator.c | $(OBJ_DIR)
	@echo "Compiling frame_allocator.c..."
	gcc $(CFLAGS) -c $(SRC_DIR)/frame_allocator.c -o $@

$(OBJ_DIR)/panic.o: $(SRC_DIR)/panic.c | $(OBJ_DIR)
	@echo "Compiling panic.c..."
	gcc $(CFLAGS) -c $< -o $@


# Assemble paging.asm
$(OBJ_DIR)/paging_asm.o: $(ASM_DIR)/paging.asm | $(OBJ_DIR)
	@echo "Assembling paging.asm..."
	nasm -f elf32 $(ASM_DIR)/paging.asm -o $@

# Link all objects into the kernel ELF
kernel.elf: $(OBJS)
	@echo "Linking kernel.elf..."
	ld -m elf_i386 $(LDFLAGS) -o $@ $(OBJS) --oformat elf32-i386

# Convert ELF to binary for boot
kernel.bin: kernel.elf
	@echo "Generating kernel.bin..."
	objcopy -O binary kernel.elf kernel.bin

# Assemble bootloader and combine with kernel
os-image: $(BOOT_DIR)/boot.asm kernel.bin
	@echo "Assembling bootloader..."
	nasm -f bin $(BOOT_DIR)/boot.asm -o boot.bin
	@echo "Building OS image..."
	cat boot.bin kernel.bin > os-image

# Run using QEMU
run: os-image
	@echo "Running OS in QEMU..."
	qemu-system-i386 -fda os-image

# Clean build
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) *.bin *.elf os-image
