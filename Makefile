<<<<<<< HEAD
Build:
	/home/althafnash/Nashos/elf/bin/i686-elf-as boot.s -o boot.o
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/Keyboard.c -o keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/VGA.c -o VGA.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/Utils.c -o Utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/RCT.c -o RCT.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o keyboard.o VGA.o Utils.o terminal.o  RCT.o -lgcc
	grub-file --is-x86-multiboot myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	qemu-system-i386 -cdrom myos.iso

# Clean the build files
clean:
	rm -r boot.o
	rm -r RCT.o
	rm -r terminal.o
	rm -r Utils.o
	rm -r VGA.o
	rm -r kernel.o
	rm -r myos.bin
	rm -r myos.iso
	rm -rf isodir

# Declare the targets as 'phony' so that Make doesn't confuse them with filenames
.PHONY: clean Build
=======
# Makefile for building and cleaning NashOS

# Compiler and tools
AS = /home/althafnash/Nashos/elf/bin/i686-elf-as
CC = /home/althafnash/Nashos/elf/bin/i686-elf-gcc
LD = /home/althafnash/Nashos/elf/bin/i686-elf-gcc
GRUB_FILE = grub-file
GRUB_MKRESCUE = grub-mkrescue
QEMU = qemu-system-i386

# Files
BOOT_S = boot.s
KERNEL_C = kernel.c
LINKER_LD = linker.ld
GRUB_CFG = grub.cfg
ISO_DIR = isodir
ISO_OUTPUT = myos.iso
BIN_OUTPUT = myos.bin
BOOT_OBJ = boot.o
KERNEL_OBJ = kernel.o

# Build the OS
Build: $(BOOT_OBJ) $(KERNEL_OBJ)
	$(LD) -T $(LINKER_LD) -o $(BIN_OUTPUT) -ffreestanding -O2 -nostdlib $(BOOT_OBJ) $(KERNEL_OBJ) -lgcc
	$(GRUB_FILE) --is-x86-multiboot $(BIN_OUTPUT)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BIN_OUTPUT) $(ISO_DIR)/boot/myos.bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $(ISO_OUTPUT) $(ISO_DIR)
	$(QEMU) -cdrom $(ISO_OUTPUT)

# Compile assembly file
$(BOOT_OBJ): $(BOOT_S)
	$(AS) $(BOOT_S) -o $(BOOT_OBJ)

# Compile kernel C file
$(KERNEL_OBJ): $(KERNEL_C)
	$(CC) -c $(KERNEL_C) -o $(KERNEL_OBJ) -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Clean the build
clean:
	rm -rf $(BOOT_OBJ) $(KERNEL_OBJ) $(BIN_OUTPUT) $(ISO_OUTPUT) $(ISO_DIR)

.PHONY: Build clean
>>>>>>> 56972aa5c4b93748eb6734ac38a7bf645fbfde53
