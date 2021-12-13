#
# Makefile for DADAO-misc
#
DIR_DADAO_TOP		:= $(HOME)/DADAO
DIR_DADAO_MISC		:= $(DIR_DADAO_TOP)/DADAO-misc

DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install

#DD_GCC			:= $(DIR_DADAO_BUILD)/gcc-0903/gcc/xgcc
DD_GCC			:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-elf-gcc
DD_GAS			:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-elf-as
DD_READELF		:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-elf-readelf
DD_OBJDUMP		:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-elf-objdump

DD_QEMU_USER		:= $(DIR_DADAO_INSTALL)/bin/qemu-dadao
DD_QEMU_SYS		:= $(DIR_DADAO_INSTALL)/bin/qemu-system-dadao

all:
	@echo ""
	@echo "DADAO-misc"
	@echo ""

TEST_GAS_NAME		:= insn-simple

TESTFILE_NAME		:= conftest
TESTFILE_SRCDIR		:= $(DIR_DADAO_MISC)
TESTFILE_DSTDIR		:= $(DIR_DADAO_INSTALL)/test

TESTFILE_SRC		:= $(TESTFILE_SRCDIR)/$(TESTFILE_NAME).c
TESTFILE_DST		:= $(TESTFILE_DSTDIR)/$(TESTFILE_NAME)
TESTFILE_ELF		:= $(TESTFILE_DSTDIR)/$(TESTFILE_NAME).elf
TESTFILE_DUMP		:= $(TESTFILE_DSTDIR)/$(TESTFILE_NAME).dump

TESTFILE_CFLAGS		:= -g -O2 -save-temps
#TESTFILE_CFLAGS		:= -g -O2 -save-temps -fdump-tree-all -fdump-ipa-all -fdump-rtl-all

TESTFILE_LOG		:= $(TESTFILE_DSTDIR)/$(TESTFILE_NAME).log

TESTFILE_CFLAGS_EXTRA	:=
#TESTFILE_CFLAGS_EXTRA	:=							\
		-B$(DIR_DADAO_BUILD)/gcc-0903/./gcc/				\
		-B$(DIR_DADAO_INSTALL)/dadao-linux-elf/bin/			\
		-B$(DIR_DADAO_INSTALL)/dadao-linux-elf/lib/			\
		-isystem $(DIR_DADAO_INSTALL)/dadao-linux-elf/include		\
		-isystem $(DIR_DADAO_INSTALL)/dadao-linux-elf/sys-include	\

test-gas-highfive:
	test -d $(TESTFILE_DSTDIR) || mkdir -p $(TESTFILE_DSTDIR)
	cd $(TESTFILE_DSTDIR);							\
		$(DD_GAS)							\
			-o $(TESTFILE_DSTDIR)/$(TEST_GAS_NAME).o		\
			$(TESTFILE_SRCDIR)/$(TEST_GAS_NAME).s
	$(DD_READELF) -a $(TESTFILE_DSTDIR)/$(TEST_GAS_NAME).o > $(TESTFILE_DSTDIR)/$(TEST_GAS_NAME).elf
	$(DD_OBJDUMP) -d $(TESTFILE_DSTDIR)/$(TEST_GAS_NAME).o > $(TESTFILE_DSTDIR)/$(TEST_GAS_NAME).dump

testfile-highfive:
	test -f $(TESTFILE_SRC) || exit
	test -d $(TESTFILE_DSTDIR) || mkdir -p $(TESTFILE_DSTDIR)
	cd $(TESTFILE_DSTDIR);							\
		$(DD_GCC)							\
			$(TESTFILE_CFLAGS) $(TESTFILE_CFLAGS_EXTRA)		\
			-o $(TESTFILE_DST) $(TESTFILE_SRC)
	$(DD_READELF) -a $(TESTFILE_DST) > $(TESTFILE_ELF)
	$(DD_OBJDUMP) -d $(TESTFILE_DST) > $(TESTFILE_DUMP)

testfile-run:
	$(DD_QEMU_USER) -singlestep -strace -D $(TESTFILE_LOG) $(TESTFILE_DST)
