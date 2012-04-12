.arm
.align 2
.section .init
.global __loader_init
__loader_init:
	ldr	sp, =loader_stack_start
	push	{r0-r3}
	bl	loader_init
	bkpt
