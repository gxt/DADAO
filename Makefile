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
VER_LLVM		:= 1600
VER_LINUX		:= 0504
VER_QEMU		:= 0801
VER_CHIPYARD		:= 0107

_DADAO_CORES_		:= $(shell expr `nproc` / 2)
DADAO_PATH		:= $(DIR_DADAO_INSTALL)/bin/:$(DIR_DADAO_INSTALL)/usr/bin:/bin:/usr/bin
DADAO_MAKE		:= make --silent DIR_DADAO_TOP=$(DIR_DADAO_TOP) PATH=$(DADAO_PATH) --jobs=$(_DADAO_CORES_)
# Note the '-' at the beginning of the line which tells make to ignore any output to stderr
_DADAO_MAKEQ_		:= -$(DADAO_MAKE)

all:
	@echo ""
	@echo "DA DAO ZHI JIAN!"
	@echo "- VERSION 0.4 -"
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

tch-gnu-highfive:
	@echo "=== BUILD Toolchain dadao-linux-gnu BEGIN ==="
	@make -s BINUTILS_GDB_$(VER_BINUTILS_GDB)_TARGET=dadao-linux-gnu binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s GCC_$(VER_GCC)_TARGET=dadao-linux-gnu gcc-$(VER_GCC)-highfive
	@make -s glibc-$(VER_GLIBC)-highfive
	@echo "=== BUILD Toolchain dadao-linux-gnu DONE! ==="

tch-elf-highfive:
	@echo "=== BUILD Toolchain dadao-unknown-elf BEGIN ==="
	@make -s binutils-gdb-$(VER_BINUTILS_GDB)-highfive
	@make -s gcc-$(VER_GCC)-highfive
	@make -s newlib-cygwin-$(VER_NEWLIB_CYGWIN)-highfive
	@echo "=== BUILD Toolchain dadao-unknown-elf DONE! ==="

env-highfive:
	@echo "=== BUILD Run-Time Environment BEGIN ==="
	@make -s linux-$(VER_LINUX)-highfive
	@echo "=== BUILD Run-Time Environment DONE! ==="

dadao-qemu-highfive:
	@echo "=== BUILD BEGIN ==="
	@make --silent qemu-$(VER_QEMU)-highfive
	@echo "=== BUILD DONE! ==="
	@echo "=== TEST@qemu  BEGIN ==="
	@make --silent testset-isa-qemu-highfive
	@make --silent testset-dhrystone-qemu-highfive
	@make --silent testset-embench-qemu-highfive
	@echo "=== TEST@qemu DONE! ==="

dadao-bare-highfive:
	@echo "=== BUILD BEGIN ==="
	@make --silent chipyard-$(VER_CHIPYARD)-highfive
	@echo "=== BUILD DONE! ==="
	@echo "=== TEST@bare BEGIN ==="
	@make --silent testset-isa-bare-highfive
	@make --silent testset-dhrystone-bare-highfive
	@echo "=== TEST@bare DONE! ==="

dadao-before-highfive:
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
	@test -d $(DIR_DADAO_LOG) || mkdir -p $(DIR_DADAO_LOG)
	@test -d $(DIR_DADAO_TARGET)/include/ || mkdir --parents $(DIR_DADAO_TARGET)/include/
	@ln -sf -t $(DIR_DADAO_TARGET)/include/ $(DIR_DADAO_TOP)/DADAO-runtime/include/*

dadao-clean:
	@echo "CLEAR EVERYTHING!"
	@rm -fr __*
	@echo "CLEAR EVERYTHING DONE!"
