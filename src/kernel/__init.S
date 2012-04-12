.arm
.section .init
.global __init
__init:
	ldr	sp, =__stack_start_addr
	bl	init
	bkpt
