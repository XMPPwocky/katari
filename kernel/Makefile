include config.mk
include depend.mk

IDIR		= include
SRCDIR		= src

CSOURCES	= $(wildcard $(SRCDIR)/*.c)
OBJECTS		= $(CSOURCES:.c=.o) $(ASMSOURCES:.S=.o)

OUTPUTS		= $(OBJECTS) kernel.elf

kernel.elf:	$(OBJECTS)
	$(LD) $(LDFLAGS) -T $(LINKERSCRIPT) $^ -b $(ELF_FORMAT) -o $@

.PHONY: depend
depend:
	$(MAKEDEPEND) -- $(CFLAGS) -- -f$@ $(ALL_C_FILES)

.PHONY: clean
clean:
	rm -f $(OUTPUTS)
# DO NOT DELETE
