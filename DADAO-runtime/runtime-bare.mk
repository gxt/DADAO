
runtime-bare-run-binary:
	@cd $(CHIPYARD_0111_SOURCE)/sims/verilator;						\
		RISCV=$(DIR_DADAO_TARGET)									\
		make CONFIG=$(RUNTIME_BARE_CONFIG)							\
			BOOTROM_FILES=bootrom.dadao.img							\
			BOOTROM_FILES_DIR=$(RUNTIME_BOOTROM_TARGET)				\
			sim_dir=$(CHIPYARD_0111_BUILD)							\
			BINARY=$(RUNTIME_BARE_BINARY)							\
			run-binary

runtime-bare-tracer:
	# analyzes the contents of the omitted trace file and generates basic statistics
	@$(OPCODES_DST_DIR)/tracer.py								\
		$(CHIPYARD_0111_BUILD)/output/chipyard.TestHarness.$(RUNTIME_BARE_CONFIG)/dhrystone.out

# See chipyard/conda-reqs/circt.json for firtool version requirement
# Download site: https://github.com/llvm/circt/releases/tag/firtool-1.61.0
RUNTIME_FIRTOOL_VERSION			:= firtool-1.61.0
RUNTIME_FIRTOOL_VERSION_LOCAL	:= /pub/firtool/

runtime-bare-firtool-prepare:
	@tar xfz $(RUNTIME_FIRTOOL_VERSION_LOCAL)/firrtl-bin-linux-x64.tar.gz -C $(DIR_DADAO_INSTALL)
	@ln -sf -t $(DIR_DADAO_INSTALL)/bin $(DIR_DADAO_INSTALL)/$(RUNTIME_FIRTOOL_VERSION)/bin/*
