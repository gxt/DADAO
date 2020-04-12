#
# TOP Makefile
#
DIR_GIT_TAO		:= /pub/GIT-TAO/

DIR_DADAO		:= $(shell pwd)
DIR_DADAO_SOURCE	:= $(DIR_DADAO)/__source/
DIR_DADAO_BUILD		:= $(DIR_DADAO)/__build/
DIR_DADAO_INSTALL	:= $(DIR_DADAO)/__install/

all:
	@echo ""
	@echo "DA DAO ZHI JIAN!"
	@echo ""

tch-highfive:
	@echo "=== BUILD Toolchain BEGIN ==="
	@make -s binutils-gdb-0233-highfive
	@make -s gcc-0903-highfive
	@echo "=== BUILD Toolchain DONE! ==="

include TCH-binutils-gdb/Makefile.binutils-gdb
include TCH-gcc/Makefile.gcc-0903
include TCH-newlib-cygwin/Makefile.newlib-cygwin-0303

include DADAO-sim/Makefile.DADAO-sim
include DADAO-env/Makefile.DADAO-env

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
	@rm -fr _log-*.out _log-*.err
	@echo "CLEAR EVERYTHING DONE!"
