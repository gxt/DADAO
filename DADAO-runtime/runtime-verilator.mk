#
# Makefile for runtime-verilator
#
RUNTIME_VERILATOR_GITHUB		:= https://github.com/verilator/verilator.git

RUNTIME_VERILATOR_SOURCE		:= $(DIR_DADAO_SOURCE)/runtime-verilator
RUNTIME_VERILATOR_BUILD			:= $(DIR_DADAO_BUILD)/runtime-verilator
RUNTIME_VERILATOR_INSTALL		?= $(DIR_DADAO_INSTALL)
# See chipyard/conda-reqs/chipyard-base.yaml for verilator version requirement
RUNTIME_VERILATOR_VERSION		:= v5.020
RUNTIME_VERILATOR_BRANCH		:= dadao

RUNTIME_VERILATOR_PATCHES		:= $(DIR_DADAO_TOP)/DADAO-runtime/verilator-patches

RUNTIME_VERILATOR_LOG			:= $(DIR_DADAO_LOG)/runtime-verilator.log

runtime-verilator-clean:
	# Remove old runtime-verilator source dir ...
	@rm -fr $(RUNTIME_VERILATOR_SOURCE)
	# Remove old runtime-verilator build dir ...
	@rm -fr $(RUNTIME_VERILATOR_BUILD)

runtime-verilator-source:
	# Remove old runtime-verilator source dir ...
	@rm -fr $(RUNTIME_VERILATOR_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(RUNTIME_VERILATOR_GITHUB) -- $(RUNTIME_VERILATOR_SOURCE)
	# Checkout specified version
	@cd $(RUNTIME_VERILATOR_SOURCE); git checkout -qb $(RUNTIME_VERILATOR_BRANCH) $(RUNTIME_VERILATOR_VERSION)
	# Apply patches
	@cd $(RUNTIME_VERILATOR_SOURCE); test ! -d $(RUNTIME_VERILATOR_PATCHES) || git am $(RUNTIME_VERILATOR_PATCHES)/*.patch
	# Autoconf
	@cd $(RUNTIME_VERILATOR_SOURCE);						\
		autoconf

runtime-verilator-prepare:
	# Remove old runtime-verilator build dir ...
	@rm -fr $(RUNTIME_VERILATOR_BUILD)
	@mkdir -p $(RUNTIME_VERILATOR_BUILD)
	# Configure
	@cd $(RUNTIME_VERILATOR_BUILD);							\
		$(RUNTIME_VERILATOR_SOURCE)/configure				\
			--srcdir=$(RUNTIME_VERILATOR_SOURCE)			\
			--prefix=$(RUNTIME_VERILATOR_INSTALL)

runtime-verilator-build:
	@make $(__MAKE_J__) -C $(RUNTIME_VERILATOR_BUILD)

runtime-verilator-install:
	@make -C $(RUNTIME_VERILATOR_BUILD) install

runtime-verilator-highfive:	dadao-before-highfive
	@test ! -f $(RUNTIME_VERILATOR_LOG) || mv --force $(RUNTIME_VERILATOR_LOG) $(RUNTIME_VERILATOR_LOG).last
	@echo "=== runtime-verilator-highfive log file: $(RUNTIME_VERILATOR_LOG)"	| tee -a $(RUNTIME_VERILATOR_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"		| tee -a $(RUNTIME_VERILATOR_LOG)
	@make runtime-verilator-clean							>> $(RUNTIME_VERILATOR_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"		| tee -a $(RUNTIME_VERILATOR_LOG)
	@make runtime-verilator-source							>> $(RUNTIME_VERILATOR_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"		| tee -a $(RUNTIME_VERILATOR_LOG)
	@make runtime-verilator-prepare							>> $(RUNTIME_VERILATOR_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"		| tee -a $(RUNTIME_VERILATOR_LOG)
	@make runtime-verilator-build							>> $(RUNTIME_VERILATOR_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"		| tee -a $(RUNTIME_VERILATOR_LOG)
	@make runtime-verilator-install							>> $(RUNTIME_VERILATOR_LOG) 2>&1
	@echo "--- runtime-verilator-highfive DONE! ===         at `date +%T`"		| tee -a $(RUNTIME_VERILATOR_LOG)
