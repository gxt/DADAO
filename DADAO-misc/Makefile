#
# Makefile for DADAO-misc
#
DIR_DADAO_TOP		:= $(HOME)/DADAO

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
	@echo "DADAO-misc: test-gas-highfive test-gcc-highfive test-qemu-run"
	@echo ""

TEST_SRCDIR		:= $(DIR_DADAO_TOP)/DADAO-misc
TEST_DSTDIR		:= $(DIR_DADAO_INSTALL)/test

TEST_GAS_NAME		:= insn-simple
TEST_GAS_FLAGS		:= -o $(TEST_DSTDIR)/$(TEST_GAS_NAME).o

TEST_GCC_NAME		:= conftest
TEST_GCC_FLAGS		:= -o $(TEST_DSTDIR)/$(TEST_GCC_NAME) -g -O2 -save-temps
#TEST_GCC_FLAGS		:= -o $(TEST_DSTDIR)/$(TEST_GCC_NAME) -g -O2 -save-temps -fdump-tree-all -fdump-ipa-all -fdump-rtl-all
TEST_GCC_FLAGS_EXTRA	:=
#TEST_GCC_FLAGS_EXTRA	:=							\
		-B$(DIR_DADAO_BUILD)/gcc-0903/./gcc/				\
		-B$(DIR_DADAO_INSTALL)/dadao-linux-elf/bin/			\
		-B$(DIR_DADAO_INSTALL)/dadao-linux-elf/lib/			\
		-isystem $(DIR_DADAO_INSTALL)/dadao-linux-elf/include		\
		-isystem $(DIR_DADAO_INSTALL)/dadao-linux-elf/sys-include	\

TEST_QEMU_TARGET	:= $(TEST_DSTDIR)/$(TEST_GCC_NAME)
TEST_QEMU_LOG		:= $(TEST_DSTDIR)/$(TEST_GCC_NAME).log

test-gas-highfive:
	test -d $(TEST_DSTDIR) || mkdir -p $(TEST_DSTDIR)
	$(DD_GAS) $(TEST_GAS_FLAGS) $(TEST_SRCDIR)/$(TEST_GAS_NAME).s
	$(DD_READELF) -a $(TEST_DSTDIR)/$(TEST_GAS_NAME).o > $(TEST_DSTDIR)/$(TEST_GAS_NAME).elf
	$(DD_OBJDUMP) -d $(TEST_DSTDIR)/$(TEST_GAS_NAME).o > $(TEST_DSTDIR)/$(TEST_GAS_NAME).dump

test-gcc-highfive:
	test -d $(TEST_DSTDIR) || mkdir -p $(TEST_DSTDIR)
	$(DD_GCC) $(TEST_GCC_FLAGS) $(TEST_GCC_FLAGS_EXTRA) $(TEST_SRCDIR)/$(TEST_GCC_NAME).c
	$(DD_READELF) -a $(TEST_DSTDIR)/$(TEST_GCC_NAME).o > $(TEST_DSTDIR)/$(TEST_GCC_NAME).elf
	$(DD_OBJDUMP) -d $(TEST_DSTDIR)/$(TEST_GCC_NAME).o > $(TEST_DSTDIR)/$(TEST_GCC_NAME).dump

test-qemu-run:
	$(DD_QEMU_USER) -singlestep -strace -D $(TEST_QEMU_LOG) $(TEST_QEMU_TARGET)
