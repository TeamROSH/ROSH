; 0: Divide By Zero Exception
isr0:
    push byte 0
    push byte 0
    jmp isr_main

; 1: Debug Exception
isr1:
    push byte 0
    push byte 1
    jmp isr_main

; 2: Non Maskable Interrupt Exception
isr2:
    push byte 0
    push byte 2
    jmp isr_main

; 3: Int 3 Exception
isr3:
    push byte 0
    push byte 3
    jmp isr_main

; 4: INTO Exception
isr4:
    push byte 0
    push byte 4
    jmp isr_main

; 5: Out of Bounds Exception
isr5:
    push byte 0
    push byte 5
    jmp isr_main

; 6: Invalid Opcode Exception
isr6:
    push byte 0
    push byte 6
    jmp isr_main

; 7: Coprocessor Not Available Exception
isr7:
    push byte 0
    push byte 7
    jmp isr_main

; 8: Double Fault Exception (With Error Code!)
isr8:
    push byte 8
    jmp isr_main

; 9: Coprocessor Segment Overrun Exception
isr9:
    push byte 0
    push byte 9
    jmp isr_main

; 10: Bad TSS Exception (With Error Code!)
isr10:
    push byte 10
    jmp isr_main

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    push byte 11
    jmp isr_main

; 12: Stack Fault Exception (With Error Code!)
isr12:
    push byte 12
    jmp isr_main

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    push byte 13
    jmp isr_main

; 14: Page Fault Exception (With Error Code!)
isr14:
    push byte 14
    jmp isr_main

; 15: Reserved Exception
isr15:
    push byte 0
    push byte 15
    jmp isr_main

; 16: Floating Point Exception
isr16:
    push byte 0
    push byte 16
    jmp isr_main

; 17: Alignment Check Exception
isr17:
    push byte 0
    push byte 17
    jmp isr_main

; 18: Machine Check Exception
isr18:
    push byte 0
    push byte 18
    jmp isr_main

; 19: Reserved
isr19:
    push byte 0
    push byte 19
    jmp isr_main

; 20: Reserved
isr20:
    push byte 0
    push byte 20
    jmp isr_main

; 21: Reserved
isr21:
    push byte 0
    push byte 21
    jmp isr_main

; 22: Reserved
isr22:
    push byte 0
    push byte 22
    jmp isr_main

; 23: Reserved
isr23:
    push byte 0
    push byte 23
    jmp isr_main

; 24: Reserved
isr24:
    push byte 0
    push byte 24
    jmp isr_main

; 25: Reserved
isr25:
    push byte 0
    push byte 25
    jmp isr_main

; 26: Reserved
isr26:
    push byte 0
    push byte 26
    jmp isr_main

; 27: Reserved
isr27:
    push byte 0
    push byte 27
    jmp isr_main

; 28: Reserved
isr28:
    push byte 0
    push byte 28
    jmp isr_main

; 29: Reserved
isr29:
    push byte 0
    push byte 29
    jmp isr_main

; 30: Reserved
isr30:
    push byte 0
    push byte 30
    jmp isr_main

; 31: Reserved
isr31:
    push byte 0
    push byte 31
    jmp isr_main

; 128 (0x80): System Calls
isr128:
    push byte 0
    push byte 0		; sample
    jmp isr_main
