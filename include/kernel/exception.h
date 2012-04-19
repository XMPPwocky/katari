#pragma once

#define EXCEPTION_RESET	0
#define EXCEPTION_UND	1
#define EXCEPTION_SVC	2
#define EXCEPTION_PABT	3
#define EXCEPTION_DABT	4
#define EXCEPTION_RESERVED	5
#define EXCEPTION_IRQ	6
#define EXCEPTION_FIQ	7

extern void exception_init(void);
