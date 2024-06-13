
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

