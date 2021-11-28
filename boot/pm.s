[bits 16]
switch_pm:
	cli	 ; disable interrupts
	lgdt [gdt_descriptor]	   ; load the GDT
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax		; set cr0 to 32bit mode
	
	jmp CODE_SEG:init_pm

[bits 32]
init_pm:
	mov ax, DATA_SEG		; update registers
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov esp, KERNEL_STACK		; set kernel stack

	mov edi, KERNEL_OFFSET
    mov esi, 0x7E00   ; kernel source code
    mov ecx, 0xA000   ; 80 * 512 bytes
    rep movsb

	mov edi, USER_OFFSET
    mov esi, 0x7E00		; user code
	add esi, 0xA000
    mov ecx, 0x5E00		; 47 * 512
    rep movsb

	jmp KERNEL_OFFSET		 ; run kernel