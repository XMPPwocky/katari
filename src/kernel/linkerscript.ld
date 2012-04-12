ENTRY(__init)

SECTIONS
{
	. = __memory_start_addr;
	
	.init : { *(.init) }
	.text : { *(.text) }
	.data : { *(.rodata .data) }
	.bss  : { *(.bss) }
	
	__heap_start_addr = ALIGN(., 4);

	/* the stack increments before storing, so we aren't off by 1 here */
	__stack_start_addr = __memory_start_addr + __memory_size;
	__stack_size = 16384; /* 16K */
	__heap_size = (__stack_start_addr - __stack_size) - __heap_start_addr;
}	
