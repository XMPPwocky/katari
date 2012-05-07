.arm
.section .init
.align 2
.global __init
__init:
	/* Make sure we're in Supervisor mode,
	 * and disable interrupts during early bootup */
	cpsid	if, #0x13

	bl	__generate_global_translation_tables
	ldr	r0, =global_translation_tables
	ldr	r1, =0b01010
	orr	r0, r0, r1
	mcr	p15, 0, r0, c2, c0, 0 /* set TTBR0 (temporarily) */
	mcr	p15, 0, r0, c2, c0, 1 /* and TTBR1... */
	ldr	r0, =0
	mcr	p15, 0, r0, c2, c0, 2 /* set TTBCR... */

	mcr	p15, 0, r0, c8, c7, 0 /* clear TLBs */
	mov	r0, #1
	mcr	p15, 0, r0, c3, c0, 0 /* enable domain 0 */
	mov	r0, #0
	mcr	p15, 0, r5, c13, c0, 0 /* disable FCSE crap

	mrc	p15, 0, r0, c1, c0, 0 /* get SCTLR */
	ldr	r1, =0b1
	orr	r0, r0, r1
	mcr	p15, 0, r0, c1, c0, 0 /* Go go gadget MMU! */


	ldr	sp, =__stack_start_addr
	bl	init
	
	bkpt
	.pool

.arm
.section .init
.align 2
__generate_global_translation_tables:
	ldr	r0, =global_translation_tables
	ldr	r1, =0 /* current virtual address */
initial_faults:
	ldr	r2, =0
	ldr	r3, =__virt_start_addr
initial_faults_loop:
	cmp	r3, r1
	beq	ram_sections
	
	str	r2, [r0], #4
	add	r1, r1, #0x00100000
	b	initial_faults_loop
	.pool
ram_sections:
	ldr	r2, =__phys_start_addr /* must be 1MB-aligned */
	ldr	r3, =__virt_kernel_end_addr
	ldr	r5, =0b00010001010000001110
ram_sections_loop:
	cmp	r3, r1
	beq	trailing_faults

	orr	r6, r5, r2
	str	r6, [r0], #4

	add	r1, r1, #0x00100000
	add	r2, r2, #0x00100000
	b	ram_sections_loop
	.pool
trailing_faults:
	ldr	r2, =0
	ldr	r3, =0xFFF00000
trailing_faults_loop:
	cmp	r3, r1
	bxeq	lr

	str	r2, [r0], #4
	add	r1, r1, #0x00100000
	b	trailing_faults_loop
	.pool

.arm
.section .phys_bss
.balign 16384
.global global_translation_tables
global_translation_tables:
	.space	4 * 4096
