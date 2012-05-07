#pragma once

#include "kernel/kernel.h"
#include "kernel/vm.h"

#define	HEAP_REGION_SIZE	(4*1024*1024) /* 4MB */
#define	PAGES_PER_HEAP_REGION	(HEAP_REGION_SIZE/PAGE_SIZE)

struct HeapRegion {
	/* We use one byte per page for now *
	 * It's zero if free, if allocated it's any nonzero number 
         * two adjacent allocated regions have different (nonzero) values
	 * This is so kfree doesn't have to take more than a pointer */
	char page_data[PAGES_PER_HEAP_REGION];

	void *address;

	/* linked list goodness */ 
	struct HeapRegion *next_region;
};

extern void heap_init(void *start, size_t size);
extern void add_memory_to_heap(void *start, size_t size);
extern void new_heap_region(void *address);

extern void *kpagealloc(size_t numpages, unsigned char alignment);
extern void *kmalloc(size_t size);
extern void kfree(void *ptr);
