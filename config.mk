# USER CONFIG HERE

BOARD		= beaglebone
DEBUG		= 1

# END USER CONFIG
# Don't make changes below this line unless you know what you're doing!

ifndef CROSS_COMPILE
$(warning No cross-compilation prefix given, using default utilities...)
endif

CFLAGS		= -nostdlib -nostdinc --no-builtin -ffreestanding \
			-Wall -Wextra \
			$(patsubst %,-D%,$(C_DEFINES)) \
			-I$(IDIR) -march=$(ARCH) -mtune=$(CPU) -mfpu=$(FPU) \
			$(patsubst %,-Xassembler %, $(ASFLAGS)) \
			$(patsubst %,-Xlinker %,$(LDFLAGS))

ASFLAGS		= -I$(IDIR) -march=$(ARCH) -mcpu=$(CPU) -mfpu=$(FPU)

LDFLAGS		= $(patsubst %,--defsym %,$(SYM_DEFINES))

KERNELCFLAGS	= 
KERNELASFLAGS	= 

DEFINES		=
C_DEFINES	= $(DEFINES)
SYM_DEFINES	= $(DEFINES)

ifdef DEBUG
CFLAGS		+= -g
ASFLAGS		+= -g
LDFLAGS		+= -g
DEFINES		+= DEBUG=1
else
LDFLAGS		+= --strip-debug
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
DEFINES	+= SOC_AM335X=1
SYM_DEFINES	+= __memory_start_addr=0x80000000 __memory_size=0x10000000
endif

ifeq ($(ARCH), armv7-a)
ELF_FORMAT	= elf32-littlearm
DEFINES		+= ARCH_ARMV7A=1
endif

AS              = $(CROSS_COMPILE)as
CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
export		AS CC LD
OBJCOPY		= $(CROSS_COMPILE)objcopy
TOUCH           = touch
RM              = rm
MAKEDEPEND      = makedepend
