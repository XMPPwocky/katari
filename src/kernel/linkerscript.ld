ENTRY(__init)

SECTIONS
{
	.init __phys_start_addr : 
		{ *(.init) }
	.phys_bss (NOLOAD) :
		{ *(.phys_bss) }

	.main __virt_start_addr + SIZEOF ( .init ) + SIZEOF( .phys_bss ) : 
		AT ( ADDR( .phys_bss ) + SIZEOF ( .phys_bss ) )
		{ *(.text) *(.rodata .data) }

	.bss (NOLOAD) :
		AT ( ADDR ( .main ) + SIZEOF ( .main ) )
		{ *(.bss) }
	
	__virt_kernel_end_addr = ALIGN(., 1024*1024); /* 1MB aligned */
}	
