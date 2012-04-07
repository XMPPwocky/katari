ENTRY(__loader_init)

MEMORY
{
	ocmc : ORIGIN = 0x40300000, LENGTH = 64K

	/* note: this is the /maximum/ size- 1024M of ram is
	 * unlikely to actually be present. */
	emif : ORIGIN = 0x80000000, LENGTH = 1024M
}

SECTIONS
{
	__loader_init = ORIGIN(ocmc);
	.loader : { loader.o } > ocmc

	.kernel : { kernel.o } > emif
}	
