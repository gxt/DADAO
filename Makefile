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

DADAO-highfive:
	@echo "BEGIN TO BUILD EVERYTHING!"
	@echo "......"
	@echo "BUILD EVERYTHING DONE!"

DADAO-help:
	@make --silent tch-help
	@make --silent sim-help
	@make --silent env-help

DADAO-clean:
	@make --silent tch-clean
	@make --silent sim-clean
	@make --silent env-clean

