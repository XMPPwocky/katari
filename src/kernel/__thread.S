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
	mrc	p15, 0, r1, c13, c0, 4 /* get thread pointer from ID register */
	str	sp, [r0] /* save sp */

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
