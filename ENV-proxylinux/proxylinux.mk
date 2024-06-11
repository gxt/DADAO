#
# Makefrag for proxylinux
#
PROXYLINUX_GITHUB		:= https://github.com/riscv/riscv-pk.git

PROXYLINUX_SOURCE		:= $(DIR_DADAO_SOURCE)/proxylinux
PROXYLINUX_BUILD		:= $(DIR_DADAO_BUILD)/proxylinux
PROXYLINUX_INSTALL		?= $(DIR_DADAO_TARGET)
PROXYLINUX_VERSION		:= v1.0.0
PROXYLINUX_BRANCH		:= dadao

PROXYLINUX_PATCHES		:= $(DIR_DADAO_TOP)/ENV-proxylinux/proxylinux-patches

PROXYLINUX_LOG			:= $(DIR_DADAO_LOG)/proxylinux.log

proxylinux-clean:
	# Remove old proxylinux source dir ...
	@rm -fr $(PROXYLINUX_SOURCE)
	# Remove old proxylinux build dir ...
	@rm -fr $(PROXYLINUX_BUILD)

proxylinux-source:
	# Remove old proxylinux source dir ...
	@rm -fr $(PROXYLINUX_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(PROXYLINUX_GITHUB) -- $(PROXYLINUX_SOURCE)
	# Checkout specified version
	@cd $(PROXYLINUX_SOURCE);						\
		git checkout -qb $(PROXYLINUX_BRANCH) $(PROXYLINUX_VERSION);	\
		test ! -d $(PROXYLINUX_PATCHES) ||				\
		git am $(PROXYLINUX_PATCHES)/*.patch

proxylinux-prepare:
	# Remove old proxylinux build dir ...
	@rm -fr $(PROXYLINUX_BUILD)
	@mkdir -p $(PROXYLINUX_BUILD)
	# Configure
	@cd $(PROXYLINUX_BUILD);							\
		$(__VAR_DADAO_PATH__) $(PROXYLINUX_SOURCE)/configure		\
			--prefix=$(PROXYLINUX_INSTALL)				\
			--host=dadao-unknown-elf

proxylinux-build:
	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(PROXYLINUX_BUILD)

proxylinux-install:
	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(PROXYLINUX_BUILD) install

proxylinux-highfive:	dadao-before-highfive
	@test ! -f $(PROXYLINUX_LOG) || mv --force $(PROXYLINUX_LOG) $(PROXYLINUX_LOG).last
	@echo "=== proxylinux-highfive log file: $(PROXYLINUX_LOG)"				| tee -a $(PROXYLINUX_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(PROXYLINUX_LOG)
	@make proxylinux-clean									>> $(PROXYLINUX_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(PROXYLINUX_LOG)
	@make proxylinux-source									>> $(PROXYLINUX_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(PROXYLINUX_LOG)
	@make proxylinux-prepare								>> $(PROXYLINUX_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(PROXYLINUX_LOG)
	@make proxylinux-build									>> $(PROXYLINUX_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(PROXYLINUX_LOG)
	@make proxylinux-install								>> $(PROXYLINUX_LOG) 2>&1
	@echo "--- proxylinux-highfive DONE! ===                at `date +%T`"	| tee -a $(PROXYLINUX_LOG)
