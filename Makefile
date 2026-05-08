CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld

OBJ_DIR = build
SRC_DIR = kernel
ASM_DIR = asm

GRUB_DIR = grub
ISO_DIR = iso
GRUB_CFG = $(GRUB_DIR)/grub.cfg

# Add frame_allocator.o to object list
OBJS = $(OBJ_DIR)/boot.o \
       $(OBJ_DIR)/kernel.o \
       $(OBJ_DIR)/vga.o \
       $(OBJ_DIR)/paging.o \
       $(OBJ_DIR)/paging_asm.o \
       $(OBJ_DIR)/frame_allocator.o \
       $(OBJ_DIR)/panic.o \
       $(OBJ_DIR)/keyboard.o \
       $(OBJ_DIR)/idt.o \
       $(OBJ_DIR)/isr.o \
       $(OBJ_DIR)/isr_asm.o \
       $(OBJ_DIR)/load_idt.o \
       $(OBJ_DIR)/io.o \
       $(OBJ_DIR)/shell.o

all: iso

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(ISO_DIR)/boot/grub:
	mkdir -p $(ISO_DIR)/boot/grub

# Compile C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

# Compile assembly files
$(OBJ_DIR)/boot.o : $(ASM_DIR)/boot.asm | $(OBJ_DIR)
	nasm -f elf32 $< -o $@

$(OBJ_DIR)/%.o : $(ASM_DIR)/%.asm | $(OBJ_DIR)
	nasm -f elf32 $< -o $@

$(OBJ_DIR)/paging_asm.o : $(ASM_DIR)/paging.asm | $(OBJ_DIR)
	nasm -f elf32 $(ASM_DIR)/paging.asm -o $@

$(OBJ_DIR)/isr_asm.o : $(ASM_DIR)/isr.asm | $(OBJ_DIR)
	nasm -f elf32 $(ASM_DIR)/isr.asm -o $@

# Link the kernel
kernel.elf: $(OBJS)
	ld -m elf_i386 $(LDFLAGS) -o $@  $(OBJS) --oformat elf32-i386

# ISO
os-image: kernel.elf $(GRUB_CFG) | $(ISO_DIR)/boot/grub
	@echo "Creating ISO image..."
	cp kernel.elf $(ISO_DIR)/boot/kernel.elf
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg

iso: os-image
	grub-mkrescue -o os.iso $(ISO_DIR)
run: iso
	qemu-system-i386 -cdrom os.iso -m 512 -vga std

clean:
	rm -rf $(OBJ_DIR) *.elf os-image.iso $(ISO_DIR) 
