[bits 16]
; print a string using BIOS interrupt
; Input: bx - string address
boot_print:
	mov al, [bx]	; get char
	cmp al, 0		; if end string done
	je print_end
	
	mov ah, 0x0e	; print char
	int 0x10
	
	inc bx			; next char
	jmp boot_print

	print_end:
		ret