global _start
[bits 32]

_start:
    [extern main] ; Define main function
    call main ; Call main function
    jmp $