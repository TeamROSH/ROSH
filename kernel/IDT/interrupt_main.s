; from isr.c:
[extern isr_handler]
; from irq.c:
[extern irq_handler]

global context_jump

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

context_jump:
	add esp, 4		; clear return address

	add esp, 16		; get registers
	popad
	pushad

	sub esp, 16		; save eax, ebp
	push eax
	push ebp
	mov ebp, esp

	mov eax, [ebp + 8] ; user data segment
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax
	push eax ; user data segment
 
	mov eax, [ebp + 16]
	push eax ; current esp
	pushf

	mov eax, [ebp + 12]
	push eax ; user code segment

	mov eax, [ebp + 20]
	push eax ; jump usermode

	mov eax, [ebp + 4]
	mov ebp, [ebp]

	iret

%include "kernel/IDT/defines.s"
%include "kernel/IDT/isr_handlers.s"
%include "kernel/IDT/irq_handlers.s"