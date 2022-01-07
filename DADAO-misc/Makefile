#
# Makefile for DADAO-misc
#
DIR_DADAO_TOP		:= $(HOME)/DADAO

DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install

DD_GAS			:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-gnu-as
DD_READELF		:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-gnu-readelf
DD_OBJDUMP		:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-gnu-objdump

VER_GCC			?= 1003
DD_GCC			:= $(DIR_DADAO_INSTALL)/bin/dadao-linux-gnu-gcc
#DD_GCC			:= $(DIR_DADAO_BUILD)/gcc-$(VER_GCC)/gcc/xgcc

DD_QEMU_USER		:= $(DIR_DADAO_INSTALL)/bin/qemu-dadao
DD_QEMU_SYS		:= $(DIR_DADAO_INSTALL)/bin/qemu-system-dadao

TEST_SRCDIR		:= $(DIR_DADAO_TOP)/DADAO-misc
TEST_DSTDIR		?= $(TEST_SRCDIR)

all:
	@echo ""
	@echo "DADAO-misc: test-gas-highfive test-gcc-highfive test-qemu-run"
	@echo ""

test-clean:
	@rm -f *.i *.s *.o *.dump *.elf

TEST_GAS_NAME		:= insn-simple
TEST_GAS_FLAGS		:= -o $(TEST_DSTDIR)/$(TEST_GAS_NAME).o

test-gas-highfive:
	test -d $(TEST_DSTDIR) || mkdir -p $(TEST_DSTDIR)
	$(DD_GAS) $(TEST_GAS_FLAGS) $(TEST_SRCDIR)/$(TEST_GAS_NAME).S
	$(DD_READELF) -a $(TEST_DSTDIR)/$(TEST_GAS_NAME).o > $(TEST_DSTDIR)/$(TEST_GAS_NAME).elf
	$(DD_OBJDUMP) -d $(TEST_DSTDIR)/$(TEST_GAS_NAME).o > $(TEST_DSTDIR)/$(TEST_GAS_NAME).dump

TEST_GCC_NAME		?= conftest
TEST_GCC_FLAGS		:= -o $(TEST_DSTDIR)/$(TEST_GCC_NAME)
TEST_GCC_FLAGS		+= -save-temps
#TEST_GCC_FLAGS		+= -g -O2
#TEST_GCC_FLAGS		+= -fdump-tree-all -fdump-ipa-all -fdump-rtl-all
TEST_GCC_FLAGS_EXTRA	:=							\
		-nostdlib -lgcc							\
		-B$(DIR_DADAO_BUILD)/gcc-$(VER_GCC)/gcc/			\
		-B$(DIR_DADAO_INSTALL)/dadao-linux-gnu/bin/			\
		-B$(DIR_DADAO_INSTALL)/dadao-linux-gnu/lib/			\
		-isystem $(DIR_DADAO_INSTALL)/dadao-linux-gnu/include		\
		-isystem $(DIR_DADAO_INSTALL)/dadao-linux-gnu/sys-include

#		$(DIR_DADAO_INSTALL)/lib/gcc/dadao-linux-gnu/10.3.0/crti.o	\
#		$(DIR_DADAO_INSTALL)/lib/gcc/dadao-linux-gnu/10.3.0/crtn.o	\

test-gcc-highfive:
	test -d $(TEST_DSTDIR) || mkdir -p $(TEST_DSTDIR)
	$(DD_GCC) $(TEST_GCC_FLAGS) $(TEST_GCC_FLAGS_EXTRA) $(TEST_SRCDIR)/$(TEST_GCC_NAME).c
	$(DD_READELF) -a $(TEST_DSTDIR)/$(TEST_GCC_NAME).o > $(TEST_DSTDIR)/$(TEST_GCC_NAME).elf
	$(DD_OBJDUMP) -d $(TEST_DSTDIR)/$(TEST_GCC_NAME).o > $(TEST_DSTDIR)/$(TEST_GCC_NAME).dump

TEST_QEMU_NAME		?= conftest
TEST_QEMU_TARGET	:= $(TEST_DSTDIR)/$(TEST_QEMU_NAME)
TEST_QEMU_LOG		:= $(TEST_DSTDIR)/$(TEST_QEMU_NAME).log

test-qemu-run:
	$(DD_QEMU_USER) -singlestep -strace -D $(TEST_QEMU_LOG) $(TEST_QEMU_TARGET)
