; from isr.c:
[extern isr_handler]
; from irq.c:
[extern irq_handler]

global jump_usermode

isr_main:
	pushad			; Save State
	mov ax, ds		; save data segment
	push eax
	mov ax, 0x10	; use kernel data segment (see gdt.c)
	mov ds, ax		; update registers
	mov es, ax
	mov fs, ax
	mov gs, ax
	push esp		; push registers for registers_t argument

	cld		; clear DF flag for C
	call isr_handler	; call C handler

	pop eax			; Restore data segment
	pop eax
	mov ds, ax		; update registers
	mov es, ax
	mov fs, ax
	mov gs, ax
	popad			; Restore state
	add esp, 8		; clean pushed bytes (error, ISR number)
	iret			; pop and return

irq_main:
	pushad			; Save State
	mov ax, ds		; save data segment
	push eax
	mov ax, 0x10	; use kernel data segment (see gdt.c)
	mov ds, ax		; update registers
	mov es, ax
	mov fs, ax
	mov gs, ax
	push esp		; push registers for registers_t argument

	cld		; clear DF flag for C
	call irq_handler	; call C handler

	pop ebx			; Restore data segment
	pop ebx
	mov ds, bx		; update registers
	mov es, bx
	mov fs, bx
	mov gs, bx
	popad			; Restore state
	add esp, 8		; clean pushed bytes (error, IRQ number)
	iret			; pop and return

jump_usermode:
	pop ebx			; Restore data segment
	pop ebx
	mov ds, bx		; update registers
	mov es, bx
	mov fs, bx
	mov gs, bx
	popad			; Restore state
	add esp, 8		; clean pushed bytes (error, IRQ number)
	iret			; pop and return

%include "kernel/IDT/defines.s"
%include "kernel/IDT/isr_handlers.s"
%include "kernel/IDT/irq_handlers.s"