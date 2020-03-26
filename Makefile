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

include DADAO-tch/Makefile.DADAO-tch
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
