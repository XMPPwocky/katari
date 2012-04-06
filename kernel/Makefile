include config.mk

IDIR		= include
SRCDIR		= src

CSOURCES	= $(wildcard $(SRCDIR)/*.c)
ASMSOURCES	= $(wildcard $(SRCDIR)/*.S)
OBJECTS		= $(CSOURCES:.c=.o) $(ASMSOURCES:.S=.o)

OUTPUTS		= $(OBJECTS) kernel.elf

kernel.elf:	$(OBJECTS)
	$(LD) $(LDFLAGS) -T $(LINKERSCRIPT) $^ -b $(ELF_FORMAT) -o $@

.PHONY: depend
depend:
	$(MAKEDEPEND) -- $(CFLAGS) -- $(CSOURCES)

.PHONY: clean
clean:
	rm -f $(OUTPUTS)


# Generated code (makedepend) ahead!
# DO NOT DELETE
