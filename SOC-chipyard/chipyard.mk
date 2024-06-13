#
# Makefile for SoC in chipyard
#
CHIPYARD_0111_GITHUB		:= https://github.com/ucb-bar/chipyard.git
CHIPYARD_0111_VERSION		:= 1.11.0
CHIPYARD_0111_BRANCH		:= dadao

CHIPYARD_0111_SOURCE		:= $(DIR_DADAO_SOURCE)/chipyard-0111
CHIPYARD_0111_BUILD			:= $(DIR_DADAO_BUILD)/chipyard-0111
CHIPYARD_0111_LOG			:= $(DIR_DADAO_LOG)/chipyard-0111.log

CHIPYARD_0111_NEWFILES		:= $(DIR_DADAO_TOP)/SOC-chipyard/chipyard-0111-newfiles
CHIPYARD_0111_PATCHES		:= $(DIR_DADAO_TOP)/SOC-chipyard/chipyard-0111-patches
CHIPYARD_0111_INSTRUCTIONS	:= $(CHIPYARD_0111_NEWFILES)/generators/dadao-wuming/src/main/scala/common/instructions.scala

chipyard-0111-clean:
	@echo "Remove old chipyard source dir ..."
	@rm -fr $(CHIPYARD_0111_SOURCE)
	@echo "Remove old chipyard build dir ..."
	@rm -fr $(CHIPYARD_0111_BUILD)
	@echo "Remove old dadao-wuming build dir ..."
	@rm -fr $(CHIPYARD_0111_NEWFILES)/generators/dadao-wuming/target

chipyard-0111-source:
	@rm -fr $(CHIPYARD_0111_SOURCE)
	# Clone github repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(CHIPYARD_0111_GITHUB) -- $(CHIPYARD_0111_SOURCE)
	@cd $(CHIPYARD_0111_SOURCE); git checkout -qb $(CHIPYARD_0111_BRANCH) $(CHIPYARD_0111_VERSION)
	# Setup submodules by terribly downloading git repos from github.com
	@cd $(CHIPYARD_0111_SOURCE); $(__VAR_LOCAL_GIT__) ./scripts/init-submodules-no-riscv-tools.sh
	# Add dadao generators into chipyard
	@cp -a $(CHIPYARD_0111_NEWFILES)/generators/dadao-wuming $(CHIPYARD_0111_SOURCE)/generators/
	@cd $(CHIPYARD_0111_SOURCE);										\
		git add generators/dadao-wuming;								\
		git commit -asm"DADAO: add dadao-wuming"
	# Add dadao configs and patches
	@cp -a $(CHIPYARD_0111_NEWFILES)/generators/chipyard/src/main/scala/config/*	\
		$(CHIPYARD_0111_SOURCE)/generators/chipyard/src/main/scala/config/
	@cd $(CHIPYARD_0111_SOURCE);										\
		git add generators/chipyard/src/main/scala/config/;				\
		git commit -asm"DADAO: add chipyard-config";					\
		git am $(CHIPYARD_0111_PATCHES)/*

chipyard-0111-prepare:
	@rm -fr $(CHIPYARD_0111_BUILD)
	@mkdir -p $(CHIPYARD_0111_BUILD)
	@test -d $(RUNTIME_BOOTROM_TARGET) || make -s -C $(DIR_DADAO_TOP) runtime-bootrom-highfive
	@test -f $(DIR_DADAO_TARGET)/lib/libfesvr.so || make -s -C $(DIR_DADAO_TOP) runtime-riscv-isa-sim-highfive
	@test -f $(DIR_DADAO_INSTALL)/bin/verilator || make -s -C $(DIR_DADAO_TOP) runtime-verilator-highfive
	@test -f $(DIR_DADAO_INSTALL)/bin/firtool || make -s -C $(DIR_DADAO_TOP) runtime-bare-firtool-prepare

chipyard-0111-build-stage1:
	@cd $(CHIPYARD_0111_SOURCE)/sims/verilator;							\
		RISCV=$(DIR_DADAO_TARGET)										\
		make $(__MAKE_J__) $(__VAR_DADAO_PATH__)						\
			VERILATOR_THREADS=$(__VAR_CORES_HALF__)						\
			CONFIG=Wuming1StageConfig									\
			BOOTROM_FILES=bootrom.dadao.img								\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)					\
			sim_dir=$(CHIPYARD_0111_BUILD)

chipyard-0111-build-stage5:
	@cd $(CHIPYARD_0111_SOURCE)/sims/verilator;							\
		RISCV=$(DIR_DADAO_TARGET)										\
		make $(__MAKE_J__) $(__VAR_DADAO_PATH__)						\
			VERILATOR_THREADS=$(__VAR_CORES_HALF__)						\
			CONFIG=Wuming5StageConfig									\
			BOOTROM_FILES=bootrom.dadao.img								\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)					\
			sim_dir=$(CHIPYARD_0111_BUILD)

chipyard-0111-highfive:	dadao-before-highfive
	@test ! -f $(CHIPYARD_0111_LOG) || mv --force $(CHIPYARD_0111_LOG) $(CHIPYARD_0111_LOG).last
	@echo "=== chipyard-0111-highfive log file: $(CHIPYARD_0111_LOG)"		| tee -a $(CHIPYARD_0111_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)
	@make chipyard-0111-clean								>> $(CHIPYARD_0111_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)
	@make chipyard-0111-source								>> $(CHIPYARD_0111_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)
	@make chipyard-0111-prepare								>> $(CHIPYARD_0111_LOG) 2>&1
	@echo "--- 4.1. Build stage1                            at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)
	@make chipyard-0111-build-stage1						>> $(CHIPYARD_0111_LOG) 2>&1
	@echo "--- 4.2. Build stage5                            at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)
	@make chipyard-0111-build-stage5						>> $(CHIPYARD_0111_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)
#	@make chipyard-0111-install								>> $(CHIPYARD_0111_LOG) 2>&1
	@echo "--- chipyard-0111-highfive DONE! ===             at `date +%T`"	| tee -a $(CHIPYARD_0111_LOG)

chipyard-0111-update-instructions:
	# Generate instructions.scala
	@make -s -C $(DIR_DADAO_TOP) opcodes-bitpat
	@cat $(DIR_DADAO_TOP)/SOC-chipyard/instructions.scala-head		>  $(CHIPYARD_0111_INSTRUCTIONS)
	@cat $(OPCODES_OUTPUT_BITPAT)									>> $(CHIPYARD_0111_INSTRUCTIONS)
	@cat $(DIR_DADAO_TOP)/SOC-chipyard/instructions.scala-tail		>> $(CHIPYARD_0111_INSTRUCTIONS)
