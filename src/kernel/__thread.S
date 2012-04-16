.include "kernel/barrier.i"

.arm
.section .text
.align 2
.global gothread
/* Yields to a thread.
 * r0: struct Thread *thread
 */
gothread:
	/* save current thread's return state */
	mov	r4, r0
	srsdb	sp!, #0x13 
	bl	leavethread /* save registers */
	mov	r0, r4

	/* find the current thread to save its sp */
	mrc	p15, 0, r1, c13, c0, 3 /* get TID */
	mov	r2, #4 /* 4 bytes for a pointer to struct Thread */
	mul	r1, r2, r1 /* turn TID into an offset */
	ldr	r2, =thread_table
	add	r2, r2, r1 /* index into the thread table to get a pointer */
	str	sp, [r2] /* aand store our sp into the struct */

	/* done saving current thread, now restore state from new thread */
	ldr	r1, [r0, #4] /* Get the new thread's TID */
	mcr	p15, 0, r1, c13, c0, 3 /* save it in the thread ID register */

	ldr	sp, [r0] /* use new thread's stack */
	
	/* Restore the registers... */
	ldmia	sp, {r0-r14}^ /* caret forces use of user registers */
	add	sp, sp, #4*15 /* but sadly also disables writeback */
	/* and restore the CPSR/PC */
	rfeia	sp! /* Cowabunga! */

.arm
.section .text
.align 2
.global leavethread
/* This is called by all exception handlers.
 * It saves the thread state and does other things needed on a context switch.
 * We may be in any exception mode when we get here.
 * BEFORE CALLING THIS from an exception handler, do
 *	sub	lr, lr, #mode_specific_offset
 *	srsdb   sp!, #0x13
 * to save the SPSR and LR. Read the ARM ARM for the proper value of
 * mode_specific_offset.
 */
leavethread:
	/* Clear any exclusive accesses this CPU holds. */
	clrex

	/* Switch to Supervisor mode and disable interrupts. */
	cpsid	if, #0x13
	BARRIER
	
	stmdb	sp, {r0-r14}^
	add	sp, sp, #4*15

	bx	lr /* return to the handler, let it do its thing */
