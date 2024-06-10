Build:
	/home/althafnash/Nashos/elf/bin/i686-elf-as boot.s -o boot.o
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	/home/althafnash/Nashos/elf/bin/i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	grub-file --is-x86-multiboot myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -r kernel.o 
	rm -r myos.bin
	rm -r myos.iso 
	rm -r boot.o 
	rm -rf isodir

.PHONY:Build , clean 
