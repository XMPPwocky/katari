include config.mk
include depend.mk

IDIR		= include
SRCDIR		= src

KERNELSOURCES	= $(wildcard $(SRCDIR)/kernel/*.c)
LOADERSOURCES	= $(wildcard $(SRCDIR)/loader/*.c)

KERNELOBJECTS	= $(KERNELSOURCES:.c=.o)
LOADEROBJECTS	= $(LOADERSOURCES:.c=.o)

CLEANABLES		= $(KERNELOBJECTS) $(LOADEROBJECTS) \
		  loader.o kernel.o katari.elf loader.bin kernel.bin

katari.elf:	loader.o kernel.o
	$(LD) $(LDFLAGS) -T $(LINKERSCRIPT) $^ -b $(ELF_FORMAT) -o $@

loader.bin:	katari.elf
	$(OBJCOPY) -I $(ELF_FORMAT) -O binary --only-section .loader $< $@

kernel.bin:	katari.elf
	$(OBJCOPY) -I $(ELF_FORMAT) -O binary --only-section .kernel $< $@
kernel.o: $(KERNELOBJECTS)
	$(LD) $(LDFLAGS) -i $^ -o $@

loader.o: $(LOADEROBJECTS)
	$(LD) $(LDFLAGS) -i $^ -o $@

.PHONY: depend
depend:
	$(MAKEDEPEND) -f depend.mk -- $(CFLAGS) -- \
		$(LOADERSOURCES) $(KERNELSOURCES)

.PHONY: clean
clean:
	rm -f $(CLEANABLES)

