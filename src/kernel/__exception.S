.include "kernel/barrier.i"
.include "kernel/exception.i"

.arm
.align 2
.section .text
.global exception_init
exception_init:
	ldr	r0, =exception_vector
	mcr	p15, 0, r0, c12, c0, 0

	bx	lr

.arm
.align 7
.section .text
.global exception_vector
exception_vector:
	bkpt					/* reset, we never get this */
	ldr	pc, =exception_handler_und	/* Undefined instruction */
	ldr	pc, =exception_handler_svc	/* Supervisor call */
	ldr	pc, =exception_handler_pabt	/* Prefetch abort */
	ldr	pc, =exception_handler_dabt	/* Data abort */
	bkpt					/* Reserved */
	ldr	pc, =exception_handler_irq	/* IRQ */
	ldr	pc, =exception_handler_fiq	/* FIQ */

.arm
.section .text
.align 2
.global exception_handler_und
exception_handler_und:
	bkpt

.arm
.section .text
.align 2
.global exception_handler_svc
exception_handler_svc:
	srsia	sp!, #0x13
	bl	leave_thread
	ldr	r0, =EXCEPTION_SVC
	pop	{r4-r12, pc}

.arm
.section .text
.align 2
.global exception_handler_pabt
exception_handler_pabt:
	bkpt

.arm
.section .text
.align 2
.global exception_handler_dabt
exception_handler_dabt:
	bkpt

.arm
.section .text
.align 2
.global exception_handler_irq
exception_handler_irq:
	bkpt

.arm
.section .text
.align 2
.global exception_handler_fiq
exception_handler_fiq:
	bkpt
