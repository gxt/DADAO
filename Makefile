#
# TOP Makefile
#

DIR_DADAO		:= $(shell pwd)
GIT_DADAO		:= /pub/GIT-DADAO/

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

