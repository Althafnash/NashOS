Build:
	/home/althafnash/Nashos/elf/bin/i686-elf-as boot.s -o boot.o
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/Keyboard.c -o keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/VGA.c -o VGA.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/Utils.c -o Utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/RCT.c -o RCT.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/IDT.c -o IDT.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/ISR.c -o ISR.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -Iinclude -c src/PIC.c -o PIC.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	nasm -f elf32 idt_load.asm -o idt_load.o
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o keyboard.o VGA.o Utils.o terminal.o  RCT.o IDT.o ISR.o PIC.o idt_load.o -lgcc
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
	rm -r IDT.o
	rm -r ISR.o
	rm -r PIC.o
	rm -r idt_load.o
	rm -r myos.bin
	rm -r myos.iso
	rm -rf isodir

# Declare the targets as 'phony' so that Make doesn't confuse them with filenames
.PHONY: clean Build
