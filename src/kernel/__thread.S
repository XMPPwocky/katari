/* Low-level functions dealing with threads. */

.include "kernel/barrier.i"
.include "kernel/exceptions.i"

/* a bit of background:
 * Before switching to a thread, the current SP_svc is moved into the ARMv7
 * privileged-only Thread and Process ID Registers, and the address of
 * its ThreadState struct is saved in SP_svc. This simplifies a lot,
 * but it can be confusing. */

.arm
.section .text
.align 2
.global thread_leave
/* Saves thread's state (and restores kernel stack).
 * Meant to be called immediately after taking an exception-
 * HOWEVER! BEFORE YOU CALL THIS, do a
 *	srsia	sp!, 0x13
 * to save important state, some of which is overwritten
 * by function calls (specifically, LR.) */
thread_leave:
	clrex /* Reset any address lock the thread might have gotten */
	cpsid	if, 0x13 /* enter Supervisor mode */
	BARRIER

	stmia	sp, {r0-r14}^ /* caret forces user-mode registers */
	/* add	sp, sp, #4*15 */ /* we don't need writeback ATM */

	/* Wow, that's all state saving is. Pretty impressive. */

	mrc	p15, 0, sp, c13, c0, 4 /* restore kernel sp... */

	bx	lr /* Ta-da! */

.arm
.section .text
.align 2
.global thread_enter
/* Enters a thread. Duh.
 * r0: struct Thread *thread
 * RETURN: r0: why we returned,
 * (one of the EXCEPTION_FOOs in kernel/exceptions.[ih])
 * Note that this function secretly never returns, but exception handlers
 * pretend that it does. */
thread_enter:
	/* we're going to push a few things so that an exception handler can
	 * pretend that we returned */
	push	{r4-r12, lr}

	/* Now save our SP. */
	mcr	p15, 0, sp, c13, c0, 4

	ldr	sp, [r0] /* get struct ThreadState */
	add	sp, sp, #(4*2)+(4*15) /* get end of said struct */

	ldmdb	sp, {r0-r14}^ /* restore user registers */
	sub	sp, sp, #(4*15) /* ^ disables writeback so do it manually */

	rfedb	sp /* whee! */
