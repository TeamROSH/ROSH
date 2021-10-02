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

	call load_kernel

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET		; read and store at 0x1000
	mov dh, 16
	mov dl, [BOOT_DRIVE]
	call disk_load		; load kernel
	ret

; Boot padding
times 510 - ($-$$) db 0
dw 0xaa55