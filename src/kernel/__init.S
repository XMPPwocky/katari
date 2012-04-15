.arm
.section .init
.align 2
.global __init
__init:
	/* Make sure we're in Supervisor mode,
	 * and disable interrupts during early bootup */
	cpsid	if, #0x13

	ldr	sp, =__stack_start_addr
	bl	init
	
	bkpt
