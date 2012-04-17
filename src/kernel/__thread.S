.include "kernel/barrier.i"

.arm
.section .text
.align 2
.global yieldtothread
/* Yields to a thread.
 * r0: struct Thread *thread
 */
yieldtothread:
	/* save current thread's return state */
	mrs	r1, cpsr
	push	{lr, r1}
	mov	r4, r0
	bl	savestate /* save registers */
	mov	r0, r4

	/* done saving current thread, now restore state from new thread */
	mcr	p15, 0, r0, c13, c0, 4 /* save thread pointer to ID register */

	ldr	sp, [r0] /* use new thread's stack */
	
	/* Restore the registers... */
	ldmia	sp, {r0-r14}^ /* caret forces use of user registers */
	add	sp, sp, #4*15 /* but sadly also disables writeback */
	/* and restore the CPSR/PC */
	rfeia	sp! /* Cowabunga! */

.arm
.section .text
.align 2
.global savestate
/* This is called by all exception handlers and by yieldtothread.
 * It saves the state of the current thread to the current stack.
 * We may be in any mode when we get here.
 * BEFORE CALLING THIS from an exception handler, do
 *	sub	lr, lr, #mode_specific_offset
 *	srsdb   sp!, #0x13
 * to make the lr point to where it should and save the SPSR and LR
 * Read the ARM ARM for the proper value of mode_specific_offset. Do this
 * before srsdb.
 * BEFORE CALLING THIS manually, push the lr and cpsr.
 */
savestate:
	/* Clear any exclusive accesses this CPU holds. */
	clrex

	/* Switch to Supervisor mode and disable interrupts. */
	cpsid	if, #0x13
	BARRIER
	
	stmdb	sp, {r0-r14}^
	sub	sp, sp, #4*15

	/* save the sp into the thread struct */
	mrc     p15, 0, r0, c13, c0, 4 /* get a pointer to the struct */
	str	sp, [r0]  /* save sp */

	bx	lr


.arm
.section .text
.align 2
.global thread_state_init
/* Initializes a new thread's state.
 * r0: struct Thread *t
 * r1: value of thread LR (restored to PC when thread runs)
 * r2: value of thread CPSR
 */
thread_state_init:
	mov	r3, sp /* save original SP */
	ldr	sp, [r0] /* load new SP */

	push	{r1, r2} /* new LR and CPSR */

	ldr	r1, =0 /* initial value for registers */
	push	{r1} /* value of r0 */
	push	{r1} /* r1 */
	push	{r1} /* r2 */
	push	{r1} /* r3 */
	push	{r1} /* r4 */
	push	{r1} /* r5 */
	push	{r1} /* r6 */
	push	{r1} /* r7 */
	push	{r1} /* r8 */
	push	{r1} /* r9 */
	push	{r1} /* r10 */
	push	{r1} /* r11 */
	push	{r1} /* r12 */

	mov	sp, r3 /* restore original SP */

	bx	lr

/* These getters and setters must ONLY be used after a savestate for the thread
 * itself, and before switching away from the thread (which saves kernel state
 * instead. */

.arm
.section .text
.align 2
.global thread_getreg
/* Get the value of a thread's register.
 * r0: struct Thread *t
 * r1: 0 through 14 for the register to get
 * RETURN: r0, value of the selected register */
thread_getreg:
	ldr	r0, [r0] /* get thread stack */

	add	r1, r1, #2 /* skip over return state */
	mov	r2, #4 /* bytes per word (length of stored register) */
	mul	r1, r2, r1 /* calculate an offset from thread sp */
	add	r0, r0, r1 /* use it */

	ldr	r0, [r0] /* aaand get the value */

	bx	lr /* if you don't know what this does, get out of my asm */

.arm
.section .text
.align 2
.global thread_setreg
/* Set a thread's register
 * r0: struct Thread *t
 * r1: register to set, 0 through 14
 * r2: value to set it to */
thread_setreg:
	ldr	r0, [r0]

	add	r1, r1, #2
	mov	r2, #4
	mul	r1, r2, r1
	add	r0, r0, r1

	str	r2, [r1]

	bx lr

.arm
.section .text
.align 2
.global thread_getcpsr
/* Get a thread's CPSR.
 * r0: struct Thread *t
 * RETURN: r0: thread's CPSR. Duh. */
thread_getcpsr:
	ldr	r0, [r0]
	ldr	r0, [r0, #4] /* offset of 4 bytes skips the lr value */

	bx	lr

.arm
.section .text
.align 2
.global thread_setcpsr
/* Set a thread's CPSR.
 * r0: struct Thread *t
 * r1: new CPSR value. */
thread_setcpsr:
	ldr	r0, [r0]
	str	r1, [r0, #4]


	bx	lr

.arm
.section .text
.align 2
.global thread_getlr
/* Get a thread's LR.
 * r0: struct Thread *t
 * RETURN: r0: thread's LR. */
thread_getlr:
	ldr	r0, [r0]
	ldr	r0, [r0]

	bx	lr

.arm
.section .text
.align 2
.global thread_setlr
/* Set a thread's LR.
 * r0: struct Thread *t
 * r1: new LR value. */
thread_setlr:
	ldr	r0, [r0]
	str	r1, [r0]

	bx	lr
