ENTRY(__init)

SECTIONS
{
	. = 0x80000000; /* Start of EMIF ram */
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}	
