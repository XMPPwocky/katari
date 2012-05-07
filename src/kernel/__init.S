.arm
.section .init
.align 2
.global __init
__init:
	/* Make sure we're in Supervisor mode,
	 * and disable interrupts during early bootup */
	cpsid	if, #0x13

	/*  */
	ldr	sp, =__stack_start_addr
	bl	init
	
	bkpt
	.pool

.arm
.section .init
.align 2
__generate_global_translation_tables:
	ldr	r0, =__global_translation_tables
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
	ldr	r3, =__memory_size
	ldr	r4, =__virt_start_addr
	add	r3, r4, r3
	ldr	r5, =0b00010001010000001110
ram_sections_loop:
	cmp	r3, r1
	beq	trailing_faults

	orr	r6, r5, r2
	str	r5, [r0], #4

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
.section .init
.align 14
__global_translation_tables:
	.space	(16 * 1024 * 1024)
