#
# Makefile for runtime-riscv-isa-sim
#
RUNTIME_RISCV_ISA_SIM_GITHUB		:= https://github.com/riscv/riscv-isa-sim.git
RUNTIME_RISCV_ISA_SIM_SOURCE		:= $(DIR_DADAO_SOURCE)/runtime-riscv-isa-sim
RUNTIME_RISCV_ISA_SIM_BUILD			:= $(DIR_DADAO_BUILD)/runtime-riscv-isa-sim
RUNTIME_RISCV_ISA_SIM_INSTALL		?= $(DIR_DADAO_TARGET)
RUNTIME_RISCV_ISA_SIM_VERSION		:= v1.1.0
RUNTIME_RISCV_ISA_SIM_BRANCH		:= dadao
RUNTIME_RISCV_ISA_SIM_PATCHES		:= $(DIR_DADAO_TOP)/DADAO-runtime/riscv-isa-sim-patches
RUNTIME_RISCV_ISA_SIM_LOG			:= $(DIR_DADAO_LOG)/runtime-riscv-isa-sim.log

runtime-riscv-isa-sim-clean:
	# Remove old runtime-riscv-isa-sim source dir ...
	@rm -fr $(RUNTIME_RISCV_ISA_SIM_SOURCE)
	# Remove old runtime-riscv-isa-sim build dir ...
	@rm -fr $(RUNTIME_RISCV_ISA_SIM_BUILD)

runtime-riscv-isa-sim-source:
	# Remove old runtime-riscv-isa-sim source dir ...
	@rm -fr $(RUNTIME_RISCV_ISA_SIM_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(RUNTIME_RISCV_ISA_SIM_GITHUB) -- $(RUNTIME_RISCV_ISA_SIM_SOURCE)
	# Checkout specified version
	@cd $(RUNTIME_RISCV_ISA_SIM_SOURCE); git checkout -qb $(RUNTIME_RISCV_ISA_SIM_BRANCH) $(RUNTIME_RISCV_ISA_SIM_VERSION)
	# Apply patches
	@cd $(RUNTIME_RISCV_ISA_SIM_SOURCE); test ! -d $(RUNTIME_RISCV_ISA_SIM_PATCHES) || git am $(RUNTIME_RISCV_ISA_SIM_PATCHES)/*.patch

runtime-riscv-isa-sim-prepare:
	# Remove old runtime-riscv-isa-sim build dir ...
	@rm -fr $(RUNTIME_RISCV_ISA_SIM_BUILD)
	@mkdir -p $(RUNTIME_RISCV_ISA_SIM_BUILD)
	# Configure
	@cd $(RUNTIME_RISCV_ISA_SIM_BUILD);							\
		$(RUNTIME_RISCV_ISA_SIM_SOURCE)/configure				\
			--srcdir=$(RUNTIME_RISCV_ISA_SIM_SOURCE)			\
			--prefix=$(RUNTIME_RISCV_ISA_SIM_INSTALL)			\
			--with-boost=no --with-boost-asio=no --with-boost-regex=no

runtime-riscv-isa-sim-build:
	@make $(__MAKE_J__) -C $(RUNTIME_RISCV_ISA_SIM_BUILD)

runtime-riscv-isa-sim-install:
	@make -C $(RUNTIME_RISCV_ISA_SIM_BUILD) install

runtime-riscv-isa-sim-highfive:	dadao-before-highfive
	@test ! -f $(RUNTIME_RISCV_ISA_SIM_LOG) || mv --force $(RUNTIME_RISCV_ISA_SIM_LOG) $(RUNTIME_RISCV_ISA_SIM_LOG).last
	@echo "=== runtime-riscv-isa-sim-highfive log file: $(RUNTIME_RISCV_ISA_SIM_LOG)"	| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"				| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
	@make runtime-riscv-isa-sim-clean						>> $(RUNTIME_RISCV_ISA_SIM_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"				| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
	@make runtime-riscv-isa-sim-source						>> $(RUNTIME_RISCV_ISA_SIM_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"				| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
	@make runtime-riscv-isa-sim-prepare						>> $(RUNTIME_RISCV_ISA_SIM_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"				| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
	@make runtime-riscv-isa-sim-build						>> $(RUNTIME_RISCV_ISA_SIM_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"				| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
	@make runtime-riscv-isa-sim-install						>> $(RUNTIME_RISCV_ISA_SIM_LOG) 2>&1
	@echo "--- runtime-riscv-isa-sim-highfive DONE! ===     at `date +%T`"				| tee -a $(RUNTIME_RISCV_ISA_SIM_LOG)
