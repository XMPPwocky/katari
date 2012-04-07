# USER CONFIG HERE

BOARD		= beaglebone

# END USER CONFIG
# Don't make changes below this line unless you know what you're doing!

ifndef CROSS_COMPILE
$(warning No cross-compilation prefix given, using default utilities...)
endif

CFLAGS		= -nostdlib -nostdinc -ffreestanding \
			$(patsubst %,-D%,$(C_DEFINES)) \
			-I$(IDIR) -march=$(ARCH) -mtune=$(CPU) -mfpu=$(FPU)
ASFLAGS		=  $(patsubst %,--defsym%,$(ASM_DEFINES)) \
		   -EL -I$(IDIR) -march=$(ARCH) -mtune=$(CPU) -mfpu=$(FPU)
LDFLAGS		=

DEFINES		=
C_DEFINES	= $(DEFINES)
ASM_DEFINES	= $(DEFINES)

ifdef DEBUG
CFLAGS		+= -g
ASFLAGS		+= -g
LDFLAGS		+= -g
DEFINES		+= DEBUG=1
endif

ifeq ($(BOARD), beaglebone)
DEFINES		+= BOARD_BEAGLEBONE=1
SOC		= am335x
else
$(error Katari is currently only compatible with the BeagleBone!)
endif

ifeq ($(SOC), am335x)
CPU	= cortex-a8
ARCH	= armv7-a
FPU	= vfp3
LINKERSCRIPT = linkerscripts/am335x.ld
endif

ifeq ($(ARCH), armv7-a)
ELF_FORMAT	= elf32-littlearm
DEFINES		+= WORD_SIZE=4
endif

AS              = $(CROSS_COMPILE)as
CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
TOUCH           = touch
RM              = rm
MAKEDEPEND      = makedepend
