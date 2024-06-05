#
# Makefile for SoC in chipyard
#
CHIPYARD_0107_GITHUB		:= https://github.com/ucb-bar/chipyard.git
CHIPYARD_0107_VERSION		:= 1.7.0
CHIPYARD_0107_BRANCH		:= dadao

CHIPYARD_0107_SOURCE		:= $(DIR_DADAO_SOURCE)/chipyard-0107
CHIPYARD_0107_BUILD			:= $(DIR_DADAO_BUILD)/chipyard-0107
CHIPYARD_0107_LOG			:= $(DIR_DADAO_LOG)/chipyard-0107.log

CHIPYARD_0107_NEWFILES		:= $(DIR_DADAO_TOP)/SOC-chipyard/chipyard-0107-newfiles
CHIPYARD_0107_PATCHES		:= $(DIR_DADAO_TOP)/SOC-chipyard/chipyard-0107-patches
CHIPYARD_0107_INSTRUCTIONS	:= $(CHIPYARD_0107_NEWFILES)/generators/dadao-wuming/src/main/scala/common/instructions.scala

chipyard-0107-clean:
	@echo "Remove old chipyard source dir ..."
	@rm -fr $(CHIPYARD_0107_SOURCE)
	@echo "Remove old chipyard build dir ..."
	@rm -fr $(CHIPYARD_0107_BUILD)
	@echo "Remove old dadao-wuming build dir ..."
	@rm -fr $(CHIPYARD_0107_NEWFILES)/generators/dadao-wuming/target

chipyard-0107-update-instructions:
	# Generate instructions.scala
	@make -s -C $(DIR_DADAO_TOP) opcodes-bitpat
	@cat $(DIR_DADAO_TOP)/SOC-chipyard/instructions.scala-head		>  $(CHIPYARD_0107_INSTRUCTIONS)
	@cat $(OPCODES_OUTPUT_BITPAT)									>> $(CHIPYARD_0107_INSTRUCTIONS)
	@cat $(DIR_DADAO_TOP)/SOC-chipyard/instructions.scala-tail		>> $(CHIPYARD_0107_INSTRUCTIONS)

chipyard-0107-source:
	@rm -fr $(CHIPYARD_0107_SOURCE)
	# Clone github repo
	@$(__VAR_L__) git clone -q $(CHIPYARD_0107_GITHUB) -- $(CHIPYARD_0107_SOURCE)
	@cd $(CHIPYARD_0107_SOURCE); git checkout -qb $(CHIPYARD_0107_BRANCH) $(CHIPYARD_0107_VERSION)
	# Setup submodules by terribly downloading git repos from github.com
	@cd $(CHIPYARD_0107_SOURCE); ./scripts/init-submodules-no-riscv-tools.sh
	# Add dadao generators into chipyard
	@ln --symbolic --target-directory=$(CHIPYARD_0107_SOURCE)/generators/ $(CHIPYARD_0107_NEWFILES)/generators/dadao-wuming
	@cd $(CHIPYARD_0107_SOURCE);								\
		git add generators/dadao-wuming;						\
		git commit -asm"DADAO: add dadao-wuming"
	# Add dadao configs and patches
	@ln --symbolic --target-directory=$(CHIPYARD_0107_SOURCE)/generators/chipyard/src/main/scala/config/	\
			$(CHIPYARD_0107_NEWFILES)/generators/chipyard/src/main/scala/config/*
	@cd $(CHIPYARD_0107_SOURCE);								\
		git add generators/chipyard/src/main/scala/config/;				\
		git commit -asm"DADAO: add chipyard-config";					\
		git am $(CHIPYARD_0107_PATCHES)/*

chipyard-0107-prepare:
	@rm -fr $(CHIPYARD_0107_BUILD)
	@mkdir -p $(CHIPYARD_0107_BUILD)
	@make -s -C $(DIR_DADAO_TOP) runtime-bootrom-highfive
	# Check necessary fesvr headers
	@test -d $(DIR_DADAO_TARGET)/include/fesvr || make -s -C $(DIR_DADAO_TOP) runtime-fesvr-highfive

chipyard-0107-build:
	@cd $(CHIPYARD_0107_SOURCE)/sims/verilator;						\
		RISCV=$(DIR_DADAO_TARGET)							\
		make -j16 VERILATOR_THREADS=8							\
			CONFIG=$(RUNTIME_BARE_CONFIG)						\
			BOOTROM_FILES=bootrom.dadao.img						\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)				\
			sim_dir=$(CHIPYARD_0107_BUILD)

chipyard-0107-highfive:	dadao-before-highfive
	@test ! -f $(CHIPYARD_0107_LOG) || mv --force $(CHIPYARD_0107_LOG) $(CHIPYARD_0107_LOG).last
	@echo "=== chipyard-0107-highfive log file: $(CHIPYARD_0107_LOG)"		| tee -a $(CHIPYARD_0107_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(CHIPYARD_0107_LOG)
	@make chipyard-0107-clean								>> $(CHIPYARD_0107_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(CHIPYARD_0107_LOG)
	@make chipyard-0107-source								>> $(CHIPYARD_0107_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(CHIPYARD_0107_LOG)
	@make chipyard-0107-prepare								>> $(CHIPYARD_0107_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(CHIPYARD_0107_LOG)
	@make chipyard-0107-build								>> $(CHIPYARD_0107_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(CHIPYARD_0107_LOG)
#	@make chipyard-0107-install								>> $(CHIPYARD_0107_LOG) 2>&1
	@echo "--- chipyard-0107-highfive DONE! ===             at `date +%T`"	| tee -a $(CHIPYARD_0107_LOG)
