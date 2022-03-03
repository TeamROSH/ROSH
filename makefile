run: clean_output compile_boot compile_libc compile_kernel compile_user build qemu
debug: clean_output compile_boot compile_libc compile_kernel compile_user build qemu_debug

clean_output:
	@rm -rf compiled/
	@mkdir compiled/
	@rm -rf objects/
	@mkdir objects/
	@mkdir objects/kernel/
	@mkdir objects/user/
	@mkdir objects/user/main/

compile_boot:
	@echo "Compiling boot..."
	@nasm -fbin boot/boot_sect.s -o compiled/boot_sect.bin

compile_libc:
	@echo "Compiling libc..."
	@i386-elf-gcc -ffreestanding -c libc/screen.c -o objects/screen.o
	@i386-elf-gcc -ffreestanding -c libc/string.c -o objects/string.o
	@i386-elf-gcc -ffreestanding -c libc/memory.c -o objects/memory.o
	@i386-elf-gcc -ffreestanding -c libc/system.c -o objects/system.o
	@i386-elf-gcc -ffreestanding -c libc/list.c -o objects/list.o
	@i386-elf-gcc -ffreestanding -c libc/rand.c -o objects/rand.o
	
compile_kernel:
	@echo "Compiling kernel..."
	@i386-elf-gcc -ffreestanding -c kernel/main/kernel_main.c -o objects/kernel/kernel_main.o
	@i386-elf-gcc -ffreestanding -c kernel/ports/ports.c -o objects/ports.o
	@i386-elf-gcc -ffreestanding -c kernel/ports/ata_pio.c -o objects/ata_pio.o
	@i386-elf-gcc -ffreestanding -c kernel/memory/paging.c -o objects/paging.o
	@i386-elf-gcc -ffreestanding -c kernel/GDT/gdt.c -o objects/gdt.o
	@i386-elf-gcc -ffreestanding -c kernel/GDT/tss.c -o objects/tss.o
	@i386-elf-gcc -ffreestanding -c kernel/IDT/time.c -o objects/time.o
	@i386-elf-gcc -ffreestanding -c kernel/IDT/keyboard.c -o objects/keyboard.o
	@i386-elf-gcc -ffreestanding -c kernel/IDT/irq.c -o objects/irq.o
	@i386-elf-gcc -ffreestanding -c kernel/IDT/idt.c -o objects/idt.o
	@i386-elf-gcc -ffreestanding -c kernel/IDT/isr.c -o objects/isr.o
	@i386-elf-gcc -ffreestanding -c kernel/syscalls/syscalls.c -o objects/syscalls.o
	@i386-elf-gcc -ffreestanding -c kernel/process/process.c -o objects/process.o
	@i386-elf-gcc -ffreestanding -c kernel/memory/heap.c -o objects/heap.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/pci.c -o objects/pci.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/ethernet_driver.c -o objects/ethernet_driver.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/ethernet.c -o objects/ethernet.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/arp.c -o objects/arp.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/protocols/ip.c -o objects/ip.o
	@i386-elf-gcc -ffreestanding -c fs/fs.c -o objects/fs.o

	@nasm kernel/main/kernel_entry.s -f elf -o objects/kernel/kernel_entry.o
	@nasm kernel/IDT/interrupt_main.s -f elf -o objects/interrupt_main.o
	@nasm kernel/GDT/load_gdt.s -f elf -o objects/load_gdt.o
	@nasm kernel/IDT/load_idt.s -f elf -o objects/load_idt.o
	@nasm kernel/GDT/flush_tss.s -f elf -o objects/flush_tss.o

	@nasm user/usermode.s -f elf -o objects/usermode.o

	@i386-elf-ld -o compiled/kernel_main.bin -Ttext 0x6400000 objects/kernel/*.o objects/*.o --oformat binary

compile_user:
	@i386-elf-gcc -ffreestanding -c user/user_main.c -o objects/user/user_main.o
	@i386-elf-gcc -ffreestanding -c user/stdlib.c -o objects/user/stdlib.o
	@i386-elf-gcc -ffreestanding -c user/commands.c -o objects/user/commands.o
	@i386-elf-gcc -ffreestanding -c user/heap.c -o objects/user/heap.o
	@i386-elf-gcc -ffreestanding -c user/syscalls/syscalls.c -o objects/user/syscalls.o
	@cp objects/memory.o objects/user/memory.o
	@cp objects/string.o objects/user/string.o

	@nasm user/usermode_entry.s -f elf -o objects/user/main/usermode_entry.o

	@i386-elf-ld -o compiled/user_main.bin -Ttext 0x7530000 objects/user/main/*.o objects/user/*.o --oformat binary

build:
ifeq (,$(wildcard ./rosh.bin))
	@echo "Drive not found. Creating..."
	@cat /dev/zero | head -c 4194304 > rosh.bin
else
	@echo "Drive found."
endif
	@dd conv=notrunc bs=1 seek=0 status=none if=compiled/boot_sect.bin of=rosh.bin
	@dd conv=notrunc bs=1 seek=512 status=none if=compiled/kernel_main.bin of=rosh.bin
	@dd conv=notrunc bs=1 seek=41472 status=none if=compiled/user_main.bin of=rosh.bin

qemu:
	@echo "Launching..."
	@qemu-system-i386 -netdev user,id=roshnet0,net=$(shell hostname -I | cut -d ' ' -f1)/24 -device rtl8139,netdev=roshnet0,id=rtl8139 -drive file=rosh.bin,index=0,format=raw

qemu_debug:
	@echo "Launching Debug..."
	@qemu-system-i386 -s -S -netdev user,id=mynet0,net=$(shell hostname -I | cut -d ' ' -f1)/24 -device rtl8139,netdev=roshnet0,id=rtl8139 -drive file=rosh.bin,index=0,format=raw