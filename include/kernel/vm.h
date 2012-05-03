#pragma once

#include "kernel/kernel.h"
#include "kernel/lock.h"

#define	PAGE_SIZE	4096
#define TTBCR_N			1

/* translation tables sizes in number of entries, not bytes! */
#define GLOBAL_TRANSLATION_TABLE_ENTRIES	4096
#define ADDRSPACE_TRANSLATION_TABLE_ENTRIES	(GLOBAL_TRANSLATION_TABLE_SIZE \
		/ ((1 << TTBCR_N)))

extern semaphore_t global_translation_tables_lock;
extern void *global_translation_tables;

enum AccessType {
	USER_READ,
	USER_WRITE,
	PRIVILEGED_READ,
	PRIVILEGED_WRITE
};

extern bool can_access(void *address, enum AccessType type);

extern uint32_t va2pa(void *address, enum AccessType type);

struct AddressSpace {
	semaphore_t lock;
	uint8_t current_asid;
	void *translation_tables; /* physical address! */
};
