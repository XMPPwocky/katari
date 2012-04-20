.arm
.section .text
.align 2
.global dostuff
dostuff:
	mov	r4, r0
_dostuff:
	cmp	r4, #0
	beq	dostuff_t1
	b	dostuff_t2

dostuff_t1:
	mov	r0, #'A'
	bl	kputc
	svc	#0
	mov	r0, #'B'
	bl	kputc
	svc	#0
	b	dostuff_t1

dostuff_t2:
	mov	r0, #'C'
	bl	kputc
	svc	#0
	mov	r0, #'D'
	bl	kputc
	svc	#0
	b	dostuff_t2
