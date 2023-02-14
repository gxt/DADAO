#
# TOP Makefile
#
DIR_DADAO_TOP		:= $(shell pwd)
DIR_DADAO_SOURCE	:= $(DIR_DADAO_TOP)/__source
DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install
DIR_DADAO_TARGET	:= $(DIR_DADAO_TOP)/__dadao
DIR_DADAO_LOG		:= $(DIR_DADAO_TOP)/__log

VER_BINUTILS_GDB	:= 0235
VER_GCC			:= 1003
VER_GLIBC		:= 0231
VER_NEWLIB_CYGWIN	:= 0303
VER_LINUX		:= 0504
VER_QEMU		:= 0600
VER_CHIPYARD		:= 0107

DADAO_PATH		:= $(DIR_DADAO_INSTALL)/bin/:$(DIR_DADAO_INSTALL)/usr/bin:/bin:/usr/bin
DADAO_MAKE		:= make --silent DIR_DADAO_TOP=$(DIR_DADAO_TOP) PATH=$(DADAO_PATH)

all:
	@echo ""
	@echo "DA DAO ZHI JIAN!"
	@echo ""

include TCH-binutils-gdb/Makefile.binutils-gdb-$(VER_BINUTILS_GDB)
include TCH-gcc/Makefile.gcc-$(VER_GCC)
include TCH-glibc/Makefile.glibc-$(VER_GLIBC)
include TCH-newlib-cygwin/Makefile.newlib-cygwin-$(VER_NEWLIB_CYGWIN)
include ENV-linux/Makefile.linux-$(VER_LINUX)
include SIM-qemu/Makefile.qemu-$(VER_QEMU)
include SOC-chipyard/Makefile.chipyard-$(VER_CHIPYARD)

include DADAO-opcodes/Makefile.opcodes
include DADAO-rte/Makefile.rte
include DADAO-tests/Makefile.tests

tch-gnu-highfive:
	@echo "=== BUILD Toolchain dadao-linux-gnu BEGIN ==="
	@make -s binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s gcc-$(VER_GCC)-highfive
	@make -s glibc-$(VER_GLIBC)-highfive
	@echo "=== BUILD Toolchain dadao-linux-gnu DONE! ==="

tch-elf-highfive:
	@echo "=== BUILD Toolchain dadao-unknown-elf BEGIN ==="
	@echo "=== building time maybe: real 11m, user 34m, sys 5m ==="
	@make -s BINUTILS_GDB_$(VER_BINUTILS_GDB)_TARGET=dadao-unknown-elf binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s GCC_$(VER_GCC)_TARGET=dadao-unknown-elf gcc-$(VER_GCC)-highfive
	@make -s newlib-cygwin-$(VER_NEWLIB_CYGWIN)-highfive
	@echo "=== BUILD Toolchain dadao-unknown-elf DONE! ==="

env-highfive:
	@echo "=== BUILD Run-Time Environment BEGIN ==="
	@make -s linux-$(VER_LINUX)-highfive
	@echo "=== BUILD Run-Time Environment DONE! ==="

sim-highfive:
	@echo "=== BUILD Simulators BEGIN ==="
	@make -s qemu-$(VER_QEMU)-highfive
	@echo "=== BUILD Simulators DONE! ==="

soc-highfive:
	@echo "=== BUILD SoC BEGIN ==="
	@make -s chipyard-$(VER_CHIPYARD)-highfive
	@echo "=== BUILD SoC DONE! ==="

dadao-highfive:
	@echo "BEGIN TO BUILD EVERYTHING!"
	@make --silent tch-elf-highfive
	@make --silent sim-highfive
	@make --silent soc-highfive
	@echo "BUILD EVERYTHING DONE!"

dadao-clean:
	@echo "CLEAR EVERYTHING!"
	@rm -fr __*
	@echo "CLEAR EVERYTHING DONE!"
