[org 0x7c00]

jmp _start

KERNEL_OFFSET equ 0x6400000		; load kernel here
KERNEL_STACK equ 0x6504FFF
USER_OFFSET equ 0x7530000		; load user here
BOOT_DRIVE db 0

; messages
MSG_RM db "Starting boot from Real Mode...", 0xa, 0xd, 0
MSG_CONSOLE db "Console initiated.", 0xa, 0xd, 0
MSG_KERNEL db "Loading Kernel from disk...", 0xa, 0xd, 0
MSG_PM db "Switching to Protected Mode...", 0xa, 0xd, 0

dapack:
        db 0x10
        db 0
.count: dw 0 ; num of sectors
.buf:   dw 0 ; memory buffer
.seg:   dw 0 ; empty
.addr:  dq 1 ; skip first sector (boot)

[bits 16]
_start:
	mov [BOOT_DRIVE], dl	; BIOS sets boot drive on dl
	
	xor ax, ax ; ax = 0
    mov es, ax
    mov ds, ax
    mov ss, ax

	; init stack
	mov bp, 0x700
	mov sp, bp

	call console_setup		; clear console

	call enable_A20
	
	print_init_messages:
		pusha
		mov bx, MSG_RM
		call boot_print
		mov bx, MSG_CONSOLE
		call boot_print
		mov bx, MSG_KERNEL
		call boot_print
		popa

	call load_kernel

	print_pm_message:
		pusha
		mov bx, MSG_PM
		call boot_print
		popa

	call switch_pm

[bits 16]
load_kernel:
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

; load functions
%include "boot/disk_load.s"
%include "boot/gdt.s"
%include "boot/pm.s"
%include "boot/boot_print.s"
%include "boot/A20.s"

; Boot padding
times 510 - ($-$$) db 0
dw 0xaa55