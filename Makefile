#
# TOP Makefile
#
DIR_DADAO_TOP		:= $(shell pwd)
DIR_DADAO_SOURCE	:= $(DIR_DADAO_TOP)/__source
DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install
DIR_DADAO_TARGET	:= $(DIR_DADAO_TOP)/__dadao
DIR_DADAO_LOG		:= $(DIR_DADAO_TOP)/__log

DIR_DADAO_TCH_ELF	:= $(DIR_DADAO_INSTALL)/__tch_elf

VER_BINUTILS_GDB	:= 0235
VER_GCC				:= 1003
VER_GLIBC			:= 0231
VER_NEWLIB_CYGWIN	:= 0303
VER_LLVM			:= 1600
VER_LINUX			:= 0504
VER_QEMU			:= 0801
VER_CHIPYARD		:= 0107

# C: CORES, P: PATH, L: LOCAL
__VAR_C__			:= $(shell expr `nproc` / 2)
__VAR_P__			:= PATH=$(DIR_DADAO_INSTALL)/bin/:$(DIR_DADAO_TCH_ELF)/bin:/bin:/usr/bin
__VAR_L__			:= GIT_CONFIG_SYSTEM=$(DIR_DADAO_TOP)/gitconfig.local

__MAKE_J__			:= --jobs=$(__VAR_C__)

all:
	@echo ""
	@echo "DA DAO ZHI JIAN!"
	@echo "- VERSION 202406 -"
	@echo ""

include DADAO-runtime/common.mk

include DADAO-opcodes/*.mk
include DADAO-runtime/runtime-*.mk
include DADAO-testset/testset-*.mk

include TCH-binutils-gdb/*.mk
include TCH-gcc/*.mk
include TCH-newlib-cygwin/*.mk
include TCH-llvm/*.mk
include SIM-qemu/*.mk
include SOC-chipyard/*.mk
include ENV-linux/*.mk
include ENV-proxylinux/*.mk

tch-elf-highfive:
	@rm -rf $(DIR_DADAO_TCH_ELF)
	@make -s binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s gcc-$(VER_GCC)-highfive
	@make -s linux-$(VER_LINUX)-headers-highfive
	@make -s newlib-cygwin-$(VER_NEWLIB_CYGWIN)-highfive
	@ln -sf -t $(DIR_DADAO_INSTALL)/bin $(DIR_DADAO_TCH_ELF)/bin/*

dadao-qemu-highfive:
	@make --silent qemu-$(VER_QEMU)-highfive
	@make --silent testset-isa-qemu-highfive
	@make --silent testset-dhrystone-qemu-highfive
	@make --silent testset-embench-qemu-highfive

dadao-bare-highfive:
	@make --silent runtime-bootrom-highfive
	@make --silent runtime-fesvr-highfive
	@make --silent chipyard-$(VER_CHIPYARD)-highfive
	@make --silent testset-isa-bare-highfive
	@make --silent testset-dhrystone-bare-highfive

dadao-before-highfive:
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
	@test -d $(DIR_DADAO_LOG) || mkdir -p $(DIR_DADAO_LOG)
	@test -d $(DIR_DADAO_INSTALL) || mkdir -p $(DIR_DADAO_INSTALL)/bin
	@test -d $(DIR_DADAO_TARGET)/include/ || mkdir --parents $(DIR_DADAO_TARGET)/include/
	@ln -sf -t $(DIR_DADAO_TARGET)/include/ $(DIR_DADAO_TOP)/DADAO-runtime/include/*

dadao-clean-all:
	@echo "=== CLEAN EVERYTING                              at `date +%T`"
	@rm -fr __*
	@echo "--- CLEAN EVERYTING DONE! ===                    at `date +%T`"
