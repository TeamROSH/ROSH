enable_A20:
	mov     ax,2403h                ;--- A20-Gate Support ---
	int     15h
	jb      a20_ns                  ;INT 15h is not supported
	cmp     ah,0
	jnz     a20_ns                  ;INT 15h is not supported
	
	mov     ax,2402h                ;--- A20-Gate Status ---
	int     15h
	jb      a20_failed              ;couldn't get status
	cmp     ah,0
	jnz     a20_failed              ;couldn't get status
	
	cmp     al,1
	jz      a20_activated           ;A20 is already activated
	
	mov     ax,2401h                ;--- A20-Gate Activate ---
	int     15h
	jb      a20_failed              ;couldn't activate the gate
	cmp     ah,0
	jnz     a20_failed              ;couldn't activate the gate

	a20_activated:
		ret

a20_failed:
	jmp $
a20_ns:
	jmp $