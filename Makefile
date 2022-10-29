#
# TOP Makefile
#
DIR_DADAO_TMP		:= /tmp/__$(USER)-DADAO/
DIR_DADAO_TOP		:= $(shell pwd)
DIR_DADAO_OPCODES	:= $(DIR_DADAO_TOP)/DADAO-opcodes/
DIR_DADAO_SOURCE	:= $(DIR_DADAO_TOP)/__source
DIR_DADAO_BUILD		:= $(DIR_DADAO_TOP)/__build
DIR_DADAO_INSTALL	:= $(DIR_DADAO_TOP)/__install
DIR_DADAO_TARGET	:= $(DIR_DADAO_TOP)/__target
DIR_DADAO_RISCV		:= $(DIR_DADAO_TOP)/__riscv
DIR_DADAO_LOG		:= $(DIR_DADAO_TOP)/__log

VER_BINUTILS_GDB	:= 0235
VER_GCC			:= 1003
VER_GLIBC		:= 0231
VER_NEWLIB_CYGWIN	:= 0303
VER_LINUX		:= 0504
VER_QEMU		:= 0600
VER_CHIPYARD		:= 0107

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

include DADAO-tests/Makefile.tests-isa
include DADAO-bench/Makefile.bench

tch-gnu-highfive:
	@echo "=== BUILD Toolchain dadao-linux-gnu BEGIN ==="
	@make -s binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s gcc-$(VER_GCC)-highfive
	@make -s glibc-$(VER_GLIBC)-highfive
	@echo "=== BUILD Toolchain dadao-linux-gnu DONE! ==="

tch-elf-highfive:
	@echo "=== BUILD Toolchain dadao-linux-elf BEGIN ==="
	@echo "=== building time maybe: real 11m, user 34m, sys 5m ==="
	@make -s BINUTILS_GDB_$(VER_BINUTILS_GDB)_TARGET=dadao-linux-elf binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s GCC_$(VER_GCC)_TARGET=dadao-linux-elf gcc-$(VER_GCC)-highfive
	@make -s newlib-cygwin-$(VER_NEWLIB_CYGWIN)-highfive
	@echo "=== BUILD Toolchain dadao-linux-elf DONE! ==="

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
	@mkdir -p $(DIR_DADAO_TMP)/__target
	@mkdir -p $(DIR_DADAO_TMP)/__riscv
	@mkdir -p $(DIR_DADAO_TMP)/__log
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__source
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__build
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__install
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__target
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__riscv
	@ln -s -t $(DIR_DADAO_TOP) $(DIR_DADAO_TMP)/__log
	@echo "Done: check $(DIR_DADAO_TMP) for all things"

dadao-clean:
	@echo "CLEAR EVERYTHING!"
	@rm -fr __*
	@rm -fr $(DIR_DADAO_TMP)
	@make --silent bench-clean
	@echo "CLEAR EVERYTHING DONE!"
