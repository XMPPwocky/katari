#pragma once

#include "kernel/kernel.h"
#include "kernel/click.h"

#define	HEAP_REGION_SIZE	65536 /* 64K */
#define	CLICKS_PER_HEAP_REGION	(HEAP_REGION_SIZE/CLICK_SIZE)

struct HeapRegion {
	/* We use one byte per click for now *
	 * It's zero if free, if allocated it's any nonzero number 
         * two adjacent allocated regions have different (nonzero) values
	 * This is so kfree doesn't have to take more than a pointer */
	char click_data[CLICKS_PER_HEAP_REGION];

	void *address;

	/* linked list goodness */ 
	struct HeapRegion *next_region;
};

extern void heap_init(void *start, size_t size);
extern void add_memory_to_heap(void *start, size_t size);
extern void new_heap_region(void *address);

extern void *kalloc(click_t size);
extern void kfree(void *ptr);
