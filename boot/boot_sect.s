[org 0x7c00]

jmp _start

KERNEL_OFFSET equ 0x1000	; load kernel here
BOOT_DRIVE db 0

[bits 16]
_start:
	mov [BOOT_DRIVE], dl	; BIOS sets boot drive on dl
	
	; init stack
	mov bp, 0x9000
	mov sp, bp

	call console_setup
	call load_kernel
	call switch_pm

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET		; read and store at 0x1000
	mov dh, 16
	mov dl, [BOOT_DRIVE]
	call disk_load		; load kernel
	ret

[bits 16]
console_setup:
	pusha
	mov ah, 0x00
	mov al, 0x03  ; text mode 80x25 16 colours
	int 0x10
	popa
	ret

[bits 32]
run_kernel:
	call KERNEL_OFFSET		; run kernel main
	jmp $

; load functions
%include "boot/disk_load.s"
%include "boot/gdt.s"
%include "boot/pm.s"

; Boot padding
times 510 - ($-$$) db 0
dw 0xaa55