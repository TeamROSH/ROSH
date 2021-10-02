[bits 16]
switch_pm:
    cli     ; disable interrupts
    lgdt [gdt_descriptor]       ; load the GDT
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax        ; set cr0 to 32bit mode
    
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    mov ax, DATA_SEG        ; update registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; update stack
    mov ebp, 0x90000
    mov esp, ebp