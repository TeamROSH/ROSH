[GLOBAL load_gdt]

load_gdt:
    mov eax, [esp+4]  
    lgdt [eax]         ; Load GDT Register with GDT 
    mov ax, 0x10
    mov ds, ax         ; Reload all the data descriptors with Data selector  
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax

    jmp 0x08:.setcs
                        ; Do the FAR JMP to next instruction to set CS with Code selector, and
                        ;    set the EIP (instruction pointer) to offset of setcs
.setcs:
	ret