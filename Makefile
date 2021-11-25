#
# TOP Makefile
#
DIR_TAO_GIT		:= /pub/GIT-TAO/
DIR_GIT_ORIGIN		:= /pub/GIT-ORIGIN/

DIR_DADAO_TMP		:= /tmp/__$(USER)-DADAO/
DIR_DADAO_TOP		:= $(shell pwd)
DIR_DADAO_OPCODES	:= $(DIR_DADAO_TOP)/DADAO-opcodes/
DIR_DADAO_SOURCE	:= $(DIR_DADAO_TOP)/__source
DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install
DIR_DADAO_LOG		:= $(DIR_DADAO_TOP)/__log

VER_BINUTILS_GDB	:= 0235
VER_GCC			:= 1003
VER_NEWLIB_CYGWIN	:= 0303
VER_LINUX		:= 0503
VER_QEMU		:= 0401

AUTORECONF		:= autoreconf

include TCH-binutils-gdb/Makefile.binutils-gdb-$(VER_BINUTILS_GDB)
include TCH-gcc/Makefile.gcc-$(VER_GCC)
include TCH-newlib-cygwin/Makefile.newlib-cygwin-$(VER_NEWLIB_CYGWIN)
include ENV-linux/Makefile.linux-$(VER_LINUX)
include SIM-qemu/Makefile.qemu-$(VER_QEMU)

all:
	@echo ""
	@echo "DA DAO ZHI JIAN!"
	@echo ""

tch-highfive:
	@echo "=== BUILD Toolchain BEGIN ==="
	@echo "=== building time maybe: real 11m, user 34m, sys 5m ==="
	@make -s binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s gcc-$(VER_GCC)-highfive
	@make -s newlib-cygwin-$(VER_NEWLIB_CYGWIN)-highfive
	@echo "=== BUILD Toolchain DONE! ==="

env-highfive:
	@echo "=== BUILD Run-Time Environment BEGIN ==="
	@make -s linux-$(VER_LINUX)-highfive
	@echo "=== BUILD Run-Time Environment DONE! ==="

sim-highfive:
	@echo "=== BUILD Simulators BEGIN ==="
	@make -s qemu-$(VER_QEMU)-highfive
	@echo "=== BUILD Simulators DONE! ==="

dadao-highfive:
	@make --silent dadao-reboot
	@echo "BEGIN TO BUILD EVERYTHING!"
	@make --silent sim-highfive
	@make --silent tch-highfive
	@make --silent env-highfive
	@echo "BUILD EVERYTHING DONE!"

dadao-reboot:
	@echo "IMPORTANT: MAKE SURE THE SERVER HAS ENOUGH MEMORY!"
	@make --silent dadao-clean
	@mkdir -p $(DIR_DADAO_TMP)/__source
	@mkdir -p $(DIR_DADAO_TMP)/__build
	@mkdir -p $(DIR_DADAO_TMP)/__install
	@mkdir -p $(DIR_DADAO_TMP)/__log
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__source
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__build
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__install
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__log
	@echo "Done: check $(DIR_DADAO_TMP) for all things"

dadao-clean:
	@echo "CLEAR EVERYTHING!"
	@rm -fr $(DIR_DADAO_SOURCE)
	@rm -fr $(DIR_DADAO_BUILD)
	@rm -fr $(DIR_DADAO_INSTALL)
	@rm -fr $(DIR_DADAO_LOG)
	@rm -fr $(DIR_DADAO_TMP)
	@echo "CLEAR EVERYTHING DONE!"
