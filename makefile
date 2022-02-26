interface:=$(shell ip addr | awk '/state UP/ {print $$2}' | head -n 1 | awk '{print substr($$0, 1, length($$0)-1)}')

run: check_libs clean_output compile_boot compile_libc compile_kernel compile_user build create_network qemu clean_network
debug: check_libs clean_output compile_boot compile_libc compile_kernel compile_user build create_network qemu_debug clean_network

check_libs:
	@sudo echo "Switched to root..."
	@bash -c "[[ \$$(command -v i386-elf-gcc) ]] || (echo One or more packages are missing. Please follow the instructions in README.md.; exit 1;)"
	@bash -c "[[ \$$(command -v nasm) ]] || (echo One or more packages are missing. Please follow the instructions in README.md.; exit 1;)"
	@bash -c "[[ \$$(command -v i386-elf-ld) ]] || (echo One or more packages are missing. Please follow the instructions in README.md.; exit 1;)"
	@bash -c "[[ \$$(command -v brctl) ]] || (echo One or more packages are missing. Please follow the instructions in README.md.; exit 1;)"
	@bash -c "[[ \$$(command -v tunctl) ]] || (echo One or more packages are missing. Please follow the instructions in README.md.; exit 1;)"

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
	@i386-elf-gcc -ffreestanding -c kernel/networking/drivers/pci.c -o objects/pci.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/drivers/ethernet_driver.c -o objects/ethernet_driver.o
	@i386-elf-gcc -ffreestanding -c kernel/networking/protocols/ethernet.c -o objects/ethernet.o
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

create_network:
	@echo "Setting up networking..."
	@sudo brctl addbr roshbr0
	@sudo ip addr flush dev $(interface)
	@sudo brctl addif roshbr0 $(interface)
	@sudo tunctl -t roshtap0 -u root > /dev/null
	@sudo brctl addif roshbr0 roshtap0
	@sudo ifconfig $(interface) up
	@sudo ifconfig roshtap0 up
	@sudo ifconfig roshbr0 up

clean_network:
	@echo "Restoring networking..."
	@sudo brctl delif roshbr0 roshtap0
	@sudo tunctl -d roshtap0 > /dev/null
	@sudo brctl delif roshbr0 $(interface)
	@sudo ifconfig roshbr0 down
	@sudo brctl delbr roshbr0
	@sudo ifconfig $(interface) up

qemu:
	@echo "Launching..."
	@sudo qemu-system-i386 -netdev tap,id=roshnet0,ifname=roshtap0,script=no,downscript=no -device rtl8139,netdev=roshnet0,id=rtl8139 -drive file=rosh.bin,index=0,format=raw

qemu_debug:
	@echo "Launching Debug..."
	@sudo qemu-system-i386 -netdev tap,id=roshnet0,ifname=roshtap0,script=no,downscript=no -device rtl8139,netdev=roshnet0,id=rtl8139 -object filter-dump,id=f1,netdev=roshnet0,file=dump.dat -drive file=rosh.bin,index=0,format=raw