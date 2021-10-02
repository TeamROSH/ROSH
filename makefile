all: compile_boot

compile_boot:
	@echo "Compiling boot..."
	@nasm -fbin boot/boot_sect.s -o compiled/boot_sect.bin

compile_kernel:
	@echo "Compiling kernel..."
	@i386-elf-gcc -ffreestanding -c kernel/kernel_main.c -o objects/kernel_main.o
	@nasm kernel/kernel_entry.s -f elf -o objects/kernel_entry.o
	@i386-elf-ld -o compiled/kernel_main.bin -Ttext 0x1000 objects/kernel_entry.o objects/kernel_main.o --oformat binary