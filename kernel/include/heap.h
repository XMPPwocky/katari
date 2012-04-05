#pragma once

#include "click.h"

#define	HEAP_REGION_SIZE	65536 /* 64K */
#define	CLICKS_PER_HEAP_REGION	(HEAP_REGION_SIZE/CLICK_SIZE)

struct HeapRegion {
	/* We use one byte per click for now */
	char free_clicks[CLICKS_PER_HEAP_REGION];

	void *address;

	/* linked list goodness */ 
	struct HeapRegion *next_region;
};

extern struct HeapRegion *heap;

extern void *kalloc(click_t size);
extern void kfree(void *ptr);
