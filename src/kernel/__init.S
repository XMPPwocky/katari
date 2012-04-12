.arm
.section .init
.global __init
__init:
	/* Make sure we're in Supervisor mode */
	mrs	r0, CPSR /* Load the CPSR into r0 */
	ldr	r1, =0xffffffe0 /* Zero mode bits */
	and	r0, r0, r1
	ldr	r1, =0x13 /* 0b10011, Supervisor mode bits */
	orr	r0, r0, r1 /* Set mode bits */
	msr	CPSR_c, r0 /* Set CPSR */

	ldr	sp, =__stack_start_addr
	bl	init
	
	bkpt
