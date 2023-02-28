#
# arch/dadao/Makefile
#
# Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
#
ifneq ($(SUBARCH),$(ARCH))
	ifeq ($(CROSS_COMPILE),)
		CROSS_COMPILE := $(call cc-cross-prefix, dadao-unknown-elf-)
	endif
endif

# Never generate .eh_frame
KBUILD_CFLAGS		+= $(call cc-option,-fno-dwarf2-cfi-asm)

CHECKFLAGS		+= -D__dadao__

head-y			:= arch/dadao/kernel/head.o

core-y			+= arch/dadao/kernel/
core-y			+= arch/dadao/mm/

libs-y			+= arch/dadao/lib/

boot			:= arch/dadao/boot/

# Default target when executing plain make
KBUILD_IMAGE		:= $(boot)/zImage

all:	zImage

zImage Image: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

define archhelp
  echo  '* zImage        - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
  echo  '  Image         - Uncompressed kernel image (arch/$(ARCH)/boot/Image)'
endef
