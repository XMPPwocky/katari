#include "kernel/kernel.h"

/* We need statically allocated space for at least one region:
 * This is because of the way add_heap_region works (allocating heap space
 * for the next heap region. */
static char heap_initial_region_data[HEAP_REGION_SIZE];
static struct HeapRegion heap_initial_region;

/* Pointer to the current start of the heap region linked list... */
static struct HeapRegion *heap = &heap_initial_region;
/* And a pointer to the end. */
static struct HeapRegion *heap_last_region = &heap_initial_region;

semaphore_t mheap_lock = 0;
static void **mheap = NULL;

/* Lock for manipulating any HeapRegion in the HeapRegion linked list. */
semaphore_t heap_lock = 0; /* Not set up yet, so lock it for safety. */

void heap_init(void *heap_start, size_t heap_size) {
	/* Set up the initial region. */
	/* Don't get the lock because heap isn't set up yet (already locked) */

	/* Set its address */
	heap_initial_region.address = (void *)&heap_initial_region_data[0];

	/* Zero its page data */
	size_t i;
	for (i = 0; i < PAGES_PER_HEAP_REGION; i++) {
		heap_initial_region.page_data[i] = 0;
	};
	
	/* And, since for now it's the only region... */
	heap_initial_region.next_region = NULL;

	/* Now that the heap is usable, do a V on the heap_lock. */
	semaphore_V(heap_lock);

	/* Now use the rest of the memory we got. */
	add_memory_to_heap(heap_start, heap_size);
};
	
void add_memory_to_heap(void *unaligned_start, size_t size) {
	void *start = ((char *)unaligned_start + \
			(PAGE_SIZE - \
			 ((unsigned long)(char *)unaligned_start) % PAGE_SIZE));
	size -= ((char *)start-(char *)unaligned_start);

	size_t num_regions = size/HEAP_REGION_SIZE;

	size_t i;
	void *pos = start;
	for (i = 0; i < num_regions; i++) {
		new_heap_region(pos);
		pos = (void *)((char *)pos)+HEAP_REGION_SIZE;
	};
};

void new_heap_region(void *address) {
	semaphore_P(heap_lock);
	
	/* Add a new heap region */

	/* Allocate a new HeapRegion */
	/* This is why we need at least one statically allocated region */
	struct HeapRegion *new_region = (struct HeapRegion *)kmalloc((sizeof (struct HeapRegion)) / PAGE_SIZE);
	
	/* Zero its page_data (mark all pages as free */
	size_t i;
	for (i = 0; i < PAGES_PER_HEAP_REGION; i++) {
		new_region->page_data[i] = 0;
	};

	new_region->address = address;
	new_region->next_region = NULL;

	/* Get the lock so we can manipulate regions in the list. */
	heap_last_region->next_region = new_region;
	heap_last_region = new_region;

	semaphore_V(heap_lock);
};
	
void *kpagealloc(size_t size) {
	semaphore_P(heap_lock);

	struct HeapRegion *current_region = heap;

	for (current_region = heap; current_region != NULL; current_region = current_region->next_region) {
		register size_t i;

		size_t run = 0; /* track "runs" of free pages */

		for (i = 0; i < PAGES_PER_HEAP_REGION; i++) {
			if (current_region->page_data[i] != 0) {
				/* not free */
				/* a non-free page breaks up any runs, so- */
				run = 0;
				continue;
			} else {
				/* This page is free */
				if (++run >= size) {
					/* and our current run is big enough
					 * for the requested size! */

					size_t run_start = (i - run) + 1;
					char prev_page_data, next_page_data;
					

					if (run_start == 0) {
						prev_page_data = 0; /* at the start of the region, there is no previos page */
					} else {
						prev_page_data = current_region->page_data[run_start - 1];
					};
					if (i == (PAGES_PER_HEAP_REGION - 1)) {
						next_page_data = 0; /* at the end of the region, there is no next page */
					} else {
						next_page_data = current_region->page_data[i + 1];
					};

					/* Find a value for our page_data that doesn't clash with neighbors */
					char our_page_data;
					if (prev_page_data == 0) {
						if (next_page_data == 1) {
							our_page_data = 2;
						} else {
							our_page_data = 1;
						};
					} else if (prev_page_data == 1) {
						if (next_page_data == 2) {
							our_page_data = 3;
						} else {
							our_page_data = 2;
						};
					} else if (prev_page_data == 2) {
						if (next_page_data == 1) {
							our_page_data = 3;
						} else {
							our_page_data = 1;
						};
					} else {
						if (next_page_data == 1) {
							our_page_data = 2;
						} else {
							our_page_data = 3;
						};
					};

					/* and- set our page_data for the run */
					register size_t j;
					for (j = run_start; j <= i; j++) {
						current_region->page_data[j] = our_page_data;
					};

					/* Ta-da! We've allocated memory! */
					/* release lock */
					semaphore_V(heap_lock);
					return (void *)((char *)current_region->address + (run_start * PAGE_SIZE));

				} else {
					/* our current run isn't big enough yet */
					continue;
				};
			};
		};


	};

	semaphore_V(heap_lock);

	return NULL;
};

void *kmalloc(size_t size) {
	return kpagealloc((size+PAGE_SIZE-1)/PAGE_SIZE);
};

static void kpagefree(void *ptr) {
	/* We may want to build a hash table of addresses to heap regions
	 * to speed up freeing
	 * (right now we have to walk the heap region list */

	semaphore_P(heap_lock);

	struct HeapRegion *current_region;
	for (current_region = heap; current_region != NULL; current_region = current_region->next_region) {
		if (!((current_region->address < ptr) & \
					(ptr < (void *)((char *)current_region->address + HEAP_REGION_SIZE)))) {
			continue; /* this region doesn't contain ptr */
		} else {
			/* this region contains ptr */

			ptrdiff_t byte_offset = (char *)ptr - (char *)current_region->address;
			size_t start_page = byte_offset / PAGE_SIZE;

			char page_data = current_region->page_data[start_page];
			
			size_t end_page;
			for (end_page = start_page;
					((end_page < PAGES_PER_HEAP_REGION) & \
					 (current_region->page_data[end_page] == page_data));
					end_page++) {
				current_region->page_data[end_page] = 0; /* mark page as free */
			};

		};
	};

	semaphore_V(heap_lock);
};

static void kmfree(void *ptr) {
	;
};
void kfree(void *ptr) {
	if ((unsigned int)ptr % PAGE_SIZE) {
		kmfree(ptr);
	} else {
		kpagefree(ptr);
	};
};
