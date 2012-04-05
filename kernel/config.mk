# USER CONFIG HERE

BOARD		= beaglebone
BOOTLOADER	= u-boot

# END USER CONFIG
# Don't make changes below this line unless you know what you're doing!

ifndef CROSS_COMPILE
$(warning No cross-compilation prefix given, using default utilities...)
endif

CFLAGS		= -nostdlib -nostdinc -ffreestanding \
			$(patsubst %,-D%,$(C_DEFINES)) \
			-I$(IDIR) -march=$(ARCH) -mtune=$(CPU) -mfpu=$(FPU)
ASFLAGS		=  -EL -I$(IDIR) -march=$(ARCH) -mtune=$(CPU) -mfpu=$(FPU)
LDFLAGS		= $(patsubst %,--defsym%,$(SYM_DEFINES))
C_DEFINES	= 
SYM_DEFINES	=

ifdef DEBUG
CFLAGS		+= -g
ASFLAGS		+= -g
LDFLAGS		+= -g
endif

ifeq ($(BOARD), beaglebone)
C_DEFINES	+= BOARD_BEAGLEBONE
SOC		= am335x
else
$(error Katari is currently only compatible with the BeagleBone!)
endif

ifeq ($(SOC), am335x)
CPU	= cortex-a8
ARCH	= armv7-a
FPU	= vfp3
endif

ifeq ($(ARCH), armv7-a)
ELF_FORMAT	= elf32-littlearm
endif

ifeq ($(BOOTLOADER), u-boot)
LINKERSCRIPT    = linkerscripts/$(SOC)_u-boot.ld
else
$(error Katari is currently only compatible with Das U-Boot.)
endif

AS              = $(CROSS_COMPILE)as
CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
TOUCH           = touch
RM              = rm
MAKEDEPEND      = makedepend
