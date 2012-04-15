.include "kernel/barrier.i"

.arm
.section .text
.align 2
.global enterthread
/* Makes another enterthread or gothread return.
 * r0: struct Thread *thread */
enterthread:
	push	{r4-r12, lr}
	ldr	sp, [r0]
	pop	{r4-r12, lr}
	bx	lr

.arm
.section .text
.align 2
.global gothread
gothread:
	push	{r4-r12, lr}

	ldr	sp, [r0] /* use new thread's stack */
	
	/* Restore the registers... */
	ldmia	sp, {r0-r14}^ /* caret forces use of user registers */
	add	sp, sp, #4*15 /* but sadly also disables writeback */
	/* and restore the CPSR/PC */
	rfeia	sp!

.arm
.section .text
.align 2
.global leavethread
/* This is called by all exception handlers.
 * It saves the thread state and does other things needed on a context switch.
 * We may be in any exception mode when we get here.
 * BEFORE CALLING THIS, do
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
