#
# Makefile for runtime-fesvr
#
RUNTIME_FESVR_GITHUB		:= https://github.com/riscv/riscv-isa-sim.git

RUNTIME_FESVR_SOURCE		:= $(DIR_DADAO_SOURCE)/runtime-fesvr
RUNTIME_FESVR_BUILD			:= $(DIR_DADAO_BUILD)/runtime-fesvr
RUNTIME_FESVR_INSTALL		?= $(DIR_DADAO_TARGET)
RUNTIME_FESVR_VERSION		:= v1.1.0
RUNTIME_FESVR_BRANCH		:= dadao

RUNTIME_FESVR_PATCHES		:= $(DIR_DADAO_TOP)/DADAO-runtime/fesvr-patches

RUNTIME_FESVR_LOG			:= $(DIR_DADAO_LOG)/runtime-fesvr.log

runtime-fesvr-clean:
	# Remove old runtime-fesvr source dir ...
	@rm -fr $(RUNTIME_FESVR_SOURCE)
	# Remove old runtime-fesvr build dir ...
	@rm -fr $(RUNTIME_FESVR_BUILD)
	# Remove old runtime-fesvr install dir ...
	@rm -fr $(RUNTIME_FESVR_INSTALL)/bin
	@rm -fr $(RUNTIME_FESVR_INSTALL)/include
	@rm -fr $(RUNTIME_FESVR_INSTALL)/lib

runtime-fesvr-source:
	# Remove old runtime-fesvr source dir ...
	@rm -fr $(RUNTIME_FESVR_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(RUNTIME_FESVR_GITHUB) -- $(RUNTIME_FESVR_SOURCE)
	# Checkout specified version
	@cd $(RUNTIME_FESVR_SOURCE); git checkout -qb $(RUNTIME_FESVR_BRANCH) $(RUNTIME_FESVR_VERSION)
	# Apply patches
	@cd $(RUNTIME_FESVR_SOURCE); git am $(RUNTIME_FESVR_PATCHES)/*.patch

runtime-fesvr-prepare:
	# Remove old runtime-fesvr build dir ...
	@rm -fr $(RUNTIME_FESVR_BUILD)
	@mkdir -p $(RUNTIME_FESVR_BUILD)
	# Configure
	@cd $(RUNTIME_FESVR_BUILD);					\
		$(RUNTIME_FESVR_SOURCE)/configure				\
			--srcdir=$(RUNTIME_FESVR_SOURCE)			\
			--prefix=$(RUNTIME_FESVR_INSTALL)

runtime-fesvr-build:
	@make $(__MAKE_J__) -C $(RUNTIME_FESVR_BUILD) libfesvr.a libfesvr.so

runtime-fesvr-install:
	@mkdir -p $(RUNTIME_FESVR_INSTALL)/lib
	@cp -a $(RUNTIME_FESVR_BUILD)/libfesvr.* $(RUNTIME_FESVR_INSTALL)/lib
	@mkdir -p $(RUNTIME_FESVR_INSTALL)/include/fesvr
	@cp -a $(RUNTIME_FESVR_BUILD)/config.h $(RUNTIME_FESVR_INSTALL)/include/fesvr
	@cp -a $(RUNTIME_FESVR_SOURCE)/fesvr/*.h $(RUNTIME_FESVR_INSTALL)/include/fesvr
#	@make -C $(RUNTIME_FESVR_BUILD) install

runtime-fesvr-highfive:	dadao-before-highfive
	@test ! -f $(RUNTIME_FESVR_LOG) || mv --force $(RUNTIME_FESVR_LOG) $(RUNTIME_FESVR_LOG).last
	@echo "=== runtime-fesvr-highfive log file: $(RUNTIME_FESVR_LOG)"		| tee -a $(RUNTIME_FESVR_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(RUNTIME_FESVR_LOG)
	@make runtime-fesvr-clean								>> $(RUNTIME_FESVR_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(RUNTIME_FESVR_LOG)
	@make runtime-fesvr-source								>> $(RUNTIME_FESVR_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(RUNTIME_FESVR_LOG)
	@make runtime-fesvr-prepare								>> $(RUNTIME_FESVR_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(RUNTIME_FESVR_LOG)
	@make runtime-fesvr-build								>> $(RUNTIME_FESVR_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(RUNTIME_FESVR_LOG)
	@make runtime-fesvr-install								>> $(RUNTIME_FESVR_LOG) 2>&1
	@echo "--- runtime-fesvr-highfive DONE! ===             at `date +%T`"	| tee -a $(RUNTIME_FESVR_LOG)
