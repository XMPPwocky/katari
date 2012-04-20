#include "kernel/kernel.h"

/* We need statically allocated space for at least one region:
 * This is because of the way add_heap_region works (allocating heap space
 * for the next heap region. */
static char heap_initial_region_data[HEAP_REGION_SIZE];
static struct HeapRegion heap_initial_region;

/* Pointer to the current start of the heap region linked list */
static struct HeapRegion *heap = &heap_initial_region;

/* And a pointer to the end */
static struct HeapRegion *heap_last_region = &heap_initial_region;

void heap_init(void *heap_start, size_t heap_size) {
	/* First, set up the initial region. */

	/* Set its address */
	heap_initial_region.address = (void *)&heap_initial_region_data[0];

	/* Zero its click data */
	click_t i;
	for (i = 0; i < CLICKS_PER_HEAP_REGION; i++) {
		heap_initial_region.click_data[i] = 0;
	};
	
	/* And, since for now it's the only region... */
	heap_initial_region.next_region = NULL;

	/* Now use the rest of the memory we got. */
	add_memory_to_heap(heap_start, heap_size);
};
	
void add_memory_to_heap(void *start, size_t size) {
	size_t num_regions = size/HEAP_REGION_SIZE;

	size_t i;
	void *pos = start;
	for (i = 0; i < num_regions; i++) {
		new_heap_region(pos);
		pos = (void *)((char *)pos)+HEAP_REGION_SIZE;
	};
};

void new_heap_region(void *address) {
	/* Add a new heap region */


	/* Allocate a new HeapRegion */
	/* This is why we need at least one statically allocated region */
	struct HeapRegion *new_region = (struct HeapRegion *)kalloc((sizeof (struct HeapRegion)) / CLICK_SIZE);
	
	/* Zero its click_data (mark all clicks as free */
	click_t i;
	for (i = 0; i < CLICKS_PER_HEAP_REGION; i++) {
		new_region->click_data[i] = 0;
	};

	new_region->address = address;
	new_region->next_region = NULL;

	heap_last_region->next_region = new_region;
	heap_last_region = new_region;
};
	
void *kalloc(click_t size) {
	struct HeapRegion *current_region = heap;

	for (current_region = heap; current_region != NULL; current_region = current_region->next_region) {
		register click_t i;

		click_t run = 0; /* track "runs" of free clicks */

		for (i = 0; i < CLICKS_PER_HEAP_REGION; i++) {
			if (current_region->click_data[i] != 0) {
				/* not free */
				/* a non-free click breaks up any runs, so- */
				run = 0;
				continue;
			} else {
				/* This click is free */
				if (++run >= size) {
					/* and our current run is big enough
					 * for the requested size! */

					click_t run_start = (i - run) + 1;
					char prev_click_data, next_click_data;
					

					if (run_start == 0) {
						prev_click_data = 0; /* at the start of the region, there is no previos click */
					} else {
						prev_click_data = current_region->click_data[run_start - 1];
					};
					if (i == (CLICKS_PER_HEAP_REGION - 1)) {
						next_click_data = 0; /* at the end of the region, there is no next click */
					} else {
						next_click_data = current_region->click_data[i + 1];
					};

					/* Find a value for our click_data that doesn't clash with neighbors */
					char our_click_data = prev_click_data + 1;
					if (our_click_data == next_click_data) {
						our_click_data++;
					};

					/* and- set our click_data for the run */
					register click_t j;
					for (j = run_start; j <= i; j++) {
						current_region->click_data[j] = our_click_data;
					};

					/* Ta-da! We've allocated memory! */
					return (void *)((char *)current_region->address + (run_start * CLICK_SIZE));

				} else {
					/* our current run isn't big enough yet */
					continue;
				};
			};
		};


	};
	return NULL;
};

void *kmalloc(size_t size) {
	return kalloc((size+CLICK_SIZE-1)/CLICK_SIZE);
};

void kfree(void *ptr) {
	/* We may want to build a hash table of addresses to heap regions
	 * to speed up freeing (right now we have to walk the heap region list */
	struct HeapRegion *current_region;
	for (current_region = heap; current_region != NULL; current_region = current_region->next_region) {
		if (!((current_region->address < ptr) & \
					(ptr < (void *)((char *)current_region->address + HEAP_REGION_SIZE)))) {
			continue; /* this region doesn't contain ptr */
		} else {
			/* this region contains ptr */

			ptrdiff_t byte_offset = (char *)ptr - (char *)current_region->address;
			click_t start_click = byte_offset / CLICK_SIZE;

			char click_data = current_region->click_data[start_click];
			
			click_t end_click;
			for (end_click = start_click;
					((end_click < CLICKS_PER_HEAP_REGION) & \
					 (current_region->click_data[end_click] == click_data));
					end_click++) {
				current_region->click_data[end_click] = 0; /* mark click as free */
			};

		};
	};
};
