ENTRY(__loader_init)

MEMORY
{
	/* The first two words here may have to be a special header
	 * containing the image size and destination address.
	 * TODO: Implement that. */
	bootimage : ORIGIN = 0x402F0000, LENGTH = 110K

	/* note: this is the /maximum/ size- 1024M of ram is
	 * unlikely to actually be present. */
	emif : ORIGIN = 0x80000000, LENGTH = 1024M
}

SECTIONS
{
	__loader_init = ORIGIN(bootimage);
	.loader : { loader.o(.text .data .bss) } > bootimage

	.kernel : { kernel.o(.text .data .bss) } > emif
}	
