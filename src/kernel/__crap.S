.arm
.section .text
.align 2
.global dostuff
dostuff:
	bl	kputc
	svc	#0
	bl	dostuff
