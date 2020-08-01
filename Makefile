#
# TOP Makefile
#
DIR_TAO_GIT		:= /pub/GIT-TAO/

DIR_DADAO_TOP		:= $(shell pwd)
DIR_DADAO_OPCODES	:= $(DIR_DADAO_TOP)/DADAO-opcodes/
DIR_DADAO_SOURCE	:= $(DIR_DADAO_TOP)/__source/
DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build/
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install/
DIR_DADAO_LOG		:= $(DIR_DADAO_TOP)/__log/

VER_BINUTILS_GDB	:= 0233
VER_GCC			:= 0903
VER_NEWLIB_CYGWIN	:= 0303
VER_LINUX		:= 0503
VER_QEMU		:= 0401

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
	@make -s dadao-clean
	@make -s binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s gcc-$(VER_GCC)-highfive
	@make -s newlib-cygwin-$(VER_NEWLIB_CYGWIN)-highfive
	@echo "=== BUILD Toolchain DONE! ==="

env-highfive:
	@echo "=== BUILD Run-Time Environment BEGIN ==="
	@make -s linux-$(VER_LINUX)-highfive
	@make -s linux-$(VER_LINUX)-headers-highfive
	@echo "=== BUILD Run-Time Environment DONE! ==="

sim-highfive:
	@echo "=== BUILD Simulators BEGIN ==="
	@make -s qemu-$(VER_QEMU)-highfive
	@echo "=== BUILD Simulators DONE! ==="

dadao-highfive:
	@make --silent dadao-clean
	@echo "BEGIN TO BUILD EVERYTHING!"
	@make --silent sim-highfive
	@make --silent tch-highfive
	@make --silent env-highfive
	@echo "BUILD EVERYTHING DONE!"

dadao-clean:
	@echo "CLEAR EVERYTHING!"
	@rm -fr $(DIR_DADAO_SOURCE)
	@rm -fr $(DIR_DADAO_BUILD)
	@rm -fr $(DIR_DADAO_INSTALL)
	@rm -fr $(DIR_DADAO_LOG)
	@echo "CLEAR EVERYTHING DONE!"
