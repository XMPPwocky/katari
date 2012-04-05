#include "click.h"
#include "heap.h"
#include "util.h"

struct HeapRegion *heap = NULL;

void *kalloc(click_t size) {
	struct HeapRegion *current_region = heap;

	while (current_region != NULL) {
		;
	};
	return NULL;
};

void kfree(void *ptr) {
	;
};
