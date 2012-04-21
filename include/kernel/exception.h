#pragma once

enum exception {
	EXCEPTION_RESET	= 0,
	EXCEPTION_UND	= 1,
	EXCEPTION_SVC	= 2,
	EXCEPTION_PABT	= 3,
	EXCEPTION_DABT	= 4,
	EXCEPTION_RESERVED	= 5,
	EXCEPTION_IRQ	= 6,
	EXCEPTION_FIQ	= 7
};

extern void exception_init(void);
