disk_load:
	pusha   ; save registers
	
	mov ax, 127		; load 127 sectors (max)
	mov [dapack.count], ax

	mov ax, 0x7E00	; set memory buffer
	mov [dapack.buf], ax

	mov dl, [BOOT_DRIVE]
	mov si, dapack
	mov ah, 0x42    ; read disk (extended)

	int 0x13        ; call BIOS interrupt
	jc disk_error   ; on error

	popa            ; get registers back
	ret

disk_error:
    jmp $