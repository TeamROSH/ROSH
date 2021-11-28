global _start

_start:
	[extern umain] ; Define main function

	jmp umain ; Call main function