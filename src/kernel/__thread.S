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
	mov	r4, r0
	srsdb	sp!, #0x13 
	bl	savestate /* save registers */
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
.global savestate
/* This is called by all exception handlers and by yieldtothread.
 * It saves the state of the current thread to the current stack.
 * We may be in any mode when we get here.
 * BEFORE CALLING THIS from an exception handler, do
 *	sub	lr, lr, #mode_specific_offset
 * to make the lr point to where it should.
 * Read the ARM ARM for the proper value of mode_specific_offset. Do this
 * before srsdb.

 * BEFORE CALLING THIS from ANYTHING, do
 *	srsdb   sp!, #0x13
 * to save the SPSR and LR.
 */
savestate:
	/* Clear any exclusive accesses this CPU holds. */
	clrex

	/* Switch to Supervisor mode and disable interrupts. */
	cpsid	if, #0x13
	BARRIER
	
	stmdb	sp, {r0-r14}^
	add	sp, sp, #4*15

	bx	lr /* return to the handler, let it do its thing */
