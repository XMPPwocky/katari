.arm
.section .text
.align 2
.global dostuff
dostuff:
	mov	r0, #0
	cps	#16
	svc	#0
