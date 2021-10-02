disk_load:
    pusha   ; save registers
    push dx

    mov ah, 0x02    ; read disk
    mov al, dh      ; number of sectors
    mov cl, 0x02    ; sector
    mov ch, 0x00    ; cylinder
    mov dh, 0x00    ; head

    int 0x13        ; call BIOS interrupt
    jc disk_error   ; on error

    pop dx

    ; BIOS sets al to number of sectors read 
    ; which has to be the number we wanted
    cmp al, dh
    jne disk_error
    popa            ; get registers back
    ret

disk_error:
    mov ah, 0x0e
    mov al, 'X'
    int 0x10
    jmp $