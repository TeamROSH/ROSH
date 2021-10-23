[GLOBAL load_idt]

load_idt:
    mov eax, [esp + 4]
    lidt eax            ;loading parameter (idt)
    ret