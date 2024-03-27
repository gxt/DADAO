#
# Makefile for SoC in chipyard
#
CHIPYARD_0107_GITHUB		:= https://github.com/ucb-bar/chipyard.git
CHIPYARD_0107_VERSION		:= 1.7.0
CHIPYARD_0107_BRANCH		:= dadao

CHIPYARD_0107_SOURCE		:= $(DIR_DADAO_SOURCE)/chipyard-0107
CHIPYARD_0107_BUILD			:= $(DIR_DADAO_BUILD)/chipyard-0107
CHIPYARD_0107_LOG_STDOUT	:= $(DIR_DADAO_LOG)/chipyard-0107.out
CHIPYARD_0107_LOG_STDERR	:= $(DIR_DADAO_LOG)/chipyard-0107.err

CHIPYARD_0107_NEWFILES		:= $(DIR_DADAO_TOP)/SOC-chipyard/chipyard-0107-newfiles
CHIPYARD_0107_PATCHES		:= $(DIR_DADAO_TOP)/SOC-chipyard/chipyard-0107-patches
CHIPYARD_0107_INSTRUCTIONS	:= $(CHIPYARD_0107_NEWFILES)/generators/dadao-wuming/src/main/scala/common/instructions.scala

#CHIPYARD_0107_DADAO_BINARY	?= $(CHIPYARD_0107_SOURCE)/generators/riscv-sodor/riscv-bmarks/dhrystone.riscv
CHIPYARD_0107_DADAO_BINARY	?= $(DIR_DADAO_TARGET)/bmarks-bare/dhrystone.dadao

chipyard-0107-clean:
	@echo "Remove old chipyard source dir ..."
	@rm -fr $(CHIPYARD_0107_SOURCE)
	@echo "Remove old chipyard build dir ..."
	@rm -fr $(CHIPYARD_0107_BUILD)
	@echo "Remove old dadao-wuming build dir ..."
	@rm -fr $(CHIPYARD_0107_NEWFILES)/generators/dadao-wuming/target

chipyard-0107-prepare:
	@make -s -C $(DIR_DADAO_TOP) opcodes-highfive
	@make -s -C $(DIR_DADAO_TOP) runtime-bootrom-highfive
	# Check necessary fesvr headers
	@test -d $(DIR_DADAO_TARGET)/include/fesvr || make -s -C $(DIR_DADAO_TOP) runtime-fesvr-highfive
	# Generate instructions.scala
	@make -s -C $(DIR_DADAO_TOP) opcodes-bitpat
	@cat $(DIR_DADAO_TOP)/SOC-chipyard/instructions.scala-head		>  $(CHIPYARD_0107_INSTRUCTIONS)
	@cat $(OPCODES_OUTPUT_BITPAT)						>> $(CHIPYARD_0107_INSTRUCTIONS)
	@cat $(DIR_DADAO_TOP)/SOC-chipyard/instructions.scala-tail		>> $(CHIPYARD_0107_INSTRUCTIONS)

chipyard-0107-source:
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
	@rm -fr $(CHIPYARD_0107_SOURCE)
	# Clone github repo
	@git clone -q $(CHIPYARD_0107_GITHUB) -- $(CHIPYARD_0107_SOURCE)
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

chipyard-0107-build-new:
	@rm -fr $(CHIPYARD_0107_BUILD)
	@mkdir -p $(CHIPYARD_0107_BUILD)

chipyard-0107-build:
	# build Wuming1StageConfig
	@cd $(CHIPYARD_0107_SOURCE)/sims/verilator;						\
		RISCV=$(DIR_DADAO_TARGET)							\
		make -j16 VERILATOR_THREADS=8							\
			CONFIG=Wuming1StageConfig						\
			BOOTROM_FILES=bootrom.dadao.img						\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)				\
			sim_dir=$(CHIPYARD_0107_BUILD)
	# build Wuming5StageConfig
	@cd $(CHIPYARD_0107_SOURCE)/sims/verilator;						\
		RISCV=$(DIR_DADAO_TARGET)							\
		make -j16 VERILATOR_THREADS=8							\
			CONFIG=Wuming5StageConfig						\
			BOOTROM_FILES=bootrom.dadao.img						\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)				\
			sim_dir=$(CHIPYARD_0107_BUILD)

chipyard-0107-run-binary-stage1:
	@cd $(CHIPYARD_0107_SOURCE)/sims/verilator;						\
		RISCV=$(DIR_DADAO_TARGET)							\
		make CONFIG=Wuming1StageConfig							\
			BOOTROM_FILES=bootrom.dadao.img						\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)				\
			sim_dir=$(CHIPYARD_0107_BUILD)						\
			BINARY=$(CHIPYARD_0107_DADAO_BINARY)					\
			run-binary

chipyard-0107-run-binary-stage5:
	@cd $(CHIPYARD_0107_SOURCE)/sims/verilator;						\
		RISCV=$(DIR_DADAO_TARGET)							\
		make CONFIG=Wuming5StageConfig							\
			BOOTROM_FILES=bootrom.dadao.img						\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)				\
			sim_dir=$(CHIPYARD_0107_BUILD)						\
			BINARY=$(CHIPYARD_0107_DADAO_BINARY)					\
			run-binary

chipyard-0107-tracer-stage1:
	# analyzes the contents of the omitted trace file and generates basic statistics
	@$(OPCODES_DST_DIR)/tracer.py								\
		$(CHIPYARD_0107_BUILD)/output/chipyard.TestHarness.Wuming1StageConfig/dhrystone.out

chipyard-0107-tracer-stage5:
	# analyzes the contents of the omitted trace file and generates basic statistics
	@$(OPCODES_DST_DIR)/tracer.py								\
		$(CHIPYARD_0107_BUILD)/output/chipyard.TestHarness.Wuming5StageConfig/dhrystone.out

chipyard-0107-highfive:
	@echo "BEGIN TO BUILD chipyard                          at `date +%T`"
	@echo "0. Remove old logfiles                           at `date +%T`"
	@test -d $(DIR_DADAO_LOG) || mkdir -p $(DIR_DADAO_LOG)
	@rm -fr $(CHIPYARD_0107_LOG_STDOUT) $(CHIPYARD_0107_LOG_STDERR)
	@echo "1. Clean old dirs                                at `date +%T`"
	@make -s chipyard-0107-clean			1>> $(CHIPYARD_0107_LOG_STDOUT) 2>> $(CHIPYARD_0107_LOG_STDERR)
	@echo "2. Prepare                                       at `date +%T`"
	@make -s chipyard-0107-prepare			1>> $(CHIPYARD_0107_LOG_STDOUT) 2>> $(CHIPYARD_0107_LOG_STDERR)
	@echo "3. New source                                    at `date +%T`"
	@make -s chipyard-0107-source			1>> $(CHIPYARD_0107_LOG_STDOUT) 2>> $(CHIPYARD_0107_LOG_STDERR)
	@echo "4. Configure                                     at `date +%T`"
	@make -s chipyard-0107-build-new		1>> $(CHIPYARD_0107_LOG_STDOUT) 2>> $(CHIPYARD_0107_LOG_STDERR)
	@echo "5. Build                                         at `date +%T`"
	@make -s chipyard-0107-build			1>> $(CHIPYARD_0107_LOG_STDOUT) 2>> $(CHIPYARD_0107_LOG_STDERR)
	# No installation needed
	@echo "BUILD chipyard DONE!                             at `date +%T`"

