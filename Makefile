CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld

OBJ_DIR = build
SRC_DIR = kernel
BOOT_DIR = bootloader
ASM_DIR = asm

GRUB_DIR = grub
ISO_DIR = iso
KERNEL_BIN = kernel.bin
GRUB_CFG = $(GRUB_DIR)/grub.cfg

# Add frame_allocator.o to object list
OBJS = $(OBJ_DIR)/kernel.o \
       $(OBJ_DIR)/vga.o \
       $(OBJ_DIR)/paging.o \
       $(OBJ_DIR)/paging_asm.o \
       $(OBJ_DIR)/frame_allocator.o\
       $(OBJ_DIR)/panic.o\
       $(OBJ_DIR)/keyboard.o\
       $(OBJ_DIR)/idt.o\
       $(OBJ_DIR)/isr.o\
       $(OBJ_DIR)/load_idt.o\
       $(OBJ_DIR)/io.o

all: os-image

# Ensure build dir exists
$(OBJ_DIR):
	@echo "Creating build directory..."
	mkdir -p $(OBJ_DIR)

# Create the ISO directory structure
$(ISO_DIR)/boot/grub:
	@echo "Creating ISO directory structure..."
	mkdir -p $(ISO_DIR)/boot/grub

# Create necessary directories under $(ISO_DIR)/boot
$(ISO_DIR)/boot:
	@echo "Creating boot directory..."
	mkdir -p $(ISO_DIR)/boot

# Create the GRUB directory if it doesn't exist
grub:
	@echo "Creating grub directory..."
	mkdir -p grub

# Create GRUB configuration file
$(GRUB_CFG): $(BOOT_DIR)/grub.cfg grub
	@echo "Generating GRUB config..."
	cp $(BOOT_DIR)/grub.cfg $(GRUB_DIR)/grub.cfg

# Copy kernel and GRUB config
$(ISO_DIR)/boot/grub/grub.cfg: $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@echo "Copying GRUB config..."
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg

$(ISO_DIR)/boot/kernel.bin: $(KERNEL_BIN) $(ISO_DIR)/boot
	@echo "Copying kernel binary..."
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin

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

$(OBJ_DIR)/keyboard.o: $(SRC_DIR)/keyboard.c | $(OBJ_DIR)
	@echo "Compiling keyboard.c..."
	gcc $(CFLAGS) -c $(SRC_DIR)/keyboard.c -o $@

$(OBJ_DIR)/idt.o: $(SRC_DIR)/idt.c | $(OBJ_DIR)
	@echo "Compiling idt.c..."
	gcc $(CFLAGS) -c $(SRC_DIR)/idt.c -o $@

$(OBJ_DIR)/isr.o: $(ASM_DIR)/isr.asm | $(OBJ_DIR)
	@echo "Assembling isr.asm..."
	nasm -f elf32 $(ASM_DIR)/isr.asm -o $@


$(OBJ_DIR)/load_idt.o: $(ASM_DIR)/load_idt.asm | $(OBJ_DIR)
	@echo "Assembling load_idt.asm..."
	nasm -f elf32 $(ASM_DIR)/load_idt.asm -o $@

$(OBJ_DIR)/io.o: $(ASM_DIR)/io.asm | $(OBJ_DIR)
	@echo "Assembling io.asm..."
	nasm -f elf32 $(ASM_DIR)/io.asm -o $@

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
os-image: $(BOOT_DIR)/boot.asm kernel.bin $(ISO_DIR)/boot/kernel.bin $(ISO_DIR)/boot/grub/grub.cfg
	@echo "Assembling bootloader..."
	nasm -f bin $(BOOT_DIR)/boot.asm -o boot.bin
	@echo "Building OS image..."
	cat boot.bin kernel.bin > $(ISO_DIR)/boot/kernel.bin
	@echo "GRUB config and kernel copied; ready for ISO generation"


# Generate the ISO image using GRUB
iso: os-image
	@echo "Generating ISO image..."
	grub-mkrescue -o os-image.iso $(ISO_DIR)

# Run using QEMU
run: os-image.iso
	@echo "Running OS in QEMU..."
	qemu-system-i386 -cdrom os-image.iso -vga std -m 512

# Clean build
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) *.bin *.elf os-image $(ISO_DIR) os-image.iso
