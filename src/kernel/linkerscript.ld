ENTRY(__init)

SECTIONS
{
	.init __phys_start_addr : 
		AT ( __phys_start_addr) 
		{ *(.init) }

	.katari __virt_start_addr + SIZEOF ( .init ) : 
		AT ( ADDR( .init ) + SIZEOF ( .init ) )
		{ *(.text) *(.rodata .data) *(.bss) }
	
	__heap_start_addr = ALIGN(., 4);
	__heap_size = ( __virt_start_addr + __memory_size ) - __heap_start_addr;
}	
