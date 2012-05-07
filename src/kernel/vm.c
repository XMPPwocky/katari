#include "kernel/kernel.h"
#include "kernel/vm.h"

semaphore_t global_translation_tables_lock = 0;

bool va_is_global(void *va) {
	return (bool)((unsigned int)va >= 0x80000000);
};

uint32_t va2pa_priv_read(void *va) {
	uint32_t result;
	asm ( 
		"mcr	p15, 0, %[va], c7, c8, 0\n\t"
		"mrc	p15, 0, %[result], c7, c4, 0\n\t"
		: [result] "=r" (result)
		: [va] "r" (va)
	);
	
	return result;
};

uint32_t va2pa_priv_write(void *va) {
	uint32_t result;
	asm ( 
		"mcr	p15, 0, %[va], c7, c8, 1\n\t"
		"mrc	p15, 0, %[result], c7, c4, 0\n\t"
		: [result] "=r" (result)
		: [va] "r" (va)
	);

	return result;
};

uint32_t va2pa_user_read(void *va) {
	uint32_t result;
	asm ( 
		"mcr	p15, 0, %[va], c7, c8, 2\n\t"
		"mrc	p15, 0, %[result], c7, c4, 0\n\t"
		: [result] "=r" (result)
		: [va] "r" (va)
	);

	return result;
};

uint32_t va2pa_user_write(void *va) {
	uint32_t result;
	asm ( 
		"mcr	p15, 0, %[va], c7, c8, 3\n\t"
		"mrc	p15, 0, %[result], c7, c4, 0\n\t"
		: [result] "=r" (result)
		: [va] "r" (va)
	);

	return result;
};
