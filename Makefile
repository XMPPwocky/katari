include config.mk
include depend.mk

IDIR		= include
SRCDIR		= src

KERNELSOURCES	= $(wildcard $(SRCDIR)/kernel/*.c)
KERNELASM	= $(wildcard $(SRCDIR)/kernel/*.S)
KERNELOBJECTS	= $(KERNELSOURCES:.c=.o) $(KERNELASM:.S=.o)

CLEANABLES		= $(KERNELOBJECTS) \
		  kernel.elf kernel.bin

kernel.bin:	kernel.elf
	$(OBJCOPY) -I $(ELF_FORMAT) -O binary $< $@

kernel.elf: $(KERNELOBJECTS)
	$(LD) $(LDFLAGS) -T $(SRCDIR)/kernel/linkerscript.ld \
		-b $(ELF_FORMAT) $^ -o $@

.PHONY: depend
depend:
	$(MAKEDEPEND) -f depend.mk -- $(CFLAGS) -- \
		$(KERNELSOURCES)

.PHONY: clean
clean:
	rm -f $(CLEANABLES)

