include config.mk

IDIR		= include
SRCDIR		= src

KERNELSOURCES	= $(wildcard $(SRCDIR)/kernel/*.c)
LOADERSOURCES	= $(wildcard $(SRCDIR)/loader/*.c)

KERNELOBJECTS	= $(KERNELSOURCES:.c=.o)
LOADEROBJECTS	= $(LOADERSOURCES:.c=.o)

CLEANABLES		= $(KERNELOBJECTS) $(LOADEROBJECTS) \
		  loader.o kernel.o katari.elf

katari.elf:	loader.o kernel.o
	$(LD) $(LDFLAGS) -T $(LINKERSCRIPT) $^ -b $(ELF_FORMAT) -o $@

kernel.o: $(KERNELOBJECTS)
	$(LD) $(LDFLAGS) -i $^ -o $@

loader.o: $(LOADEROBJECTS)
	$(LD) $(LDFLAGS) -i $^ -o $@

.PHONY: depend
depend:
	$(MAKEDEPEND) -- $(CFLAGS) -- $(CSOURCES)

.PHONY: clean
clean:
	rm -f $(CLEANABLES)


# Generated code (makedepend) ahead!
# DO NOT DELETE

src/heap.o: include/click.h include/heap.h
