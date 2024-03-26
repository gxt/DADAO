#
# Makefile for runtime-fesvr
#
RUNTIME_FESVR_LOCAL			:= /pub/GITHUB/riscv/riscv-isa-sim.git
RUNTIME_FESVR_GITHUB		:= https://github.com/riscv/riscv-isa-sim.git

RUNTIME_FESVR_SOURCE		:= $(DIR_DADAO_SOURCE)/runtime-fesvr
RUNTIME_FESVR_BUILD			:= $(DIR_DADAO_BUILD)/runtime-fesvr
RUNTIME_FESVR_INSTALL		?= $(DIR_DADAO_TARGET)
RUNTIME_FESVR_VERSION		:= v1.1.0
RUNTIME_FESVR_BRANCH		:= dadao

RUNTIME_FESVR_PATCHES		:= $(DIR_DADAO_TOP)/DADAO-runtime/fesvr-patches

RUNTIME_FESVR_LOG_STDOUT	:= $(DIR_DADAO_LOG)/runtime-fesvr.out
RUNTIME_FESVR_LOG_STDERR	:= $(DIR_DADAO_LOG)/runtime-fesvr.err

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
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
ifeq ($(wildcard $(RUNTIME_FESVR_LOCAL)),)
	# Clone remote repo
	@git clone -q $(RUNTIME_FESVR_GITHUB) -- $(RUNTIME_FESVR_SOURCE)
else
	# Clone local repo
	@git clone -q $(RUNTIME_FESVR_LOCAL) -- $(RUNTIME_FESVR_SOURCE)
endif
	# Checkout specified version
	@cd $(RUNTIME_FESVR_SOURCE); git checkout -qb $(RUNTIME_FESVR_BRANCH) $(RUNTIME_FESVR_VERSION)
	# Apply patches
	@cd $(RUNTIME_FESVR_SOURCE); git am $(RUNTIME_FESVR_PATCHES)/*.patch

runtime-fesvr-build-new:
	# Remove old runtime-fesvr build dir ...
	@rm -fr $(RUNTIME_FESVR_BUILD)
	@mkdir -p $(RUNTIME_FESVR_BUILD)
	# Configure
	@cd $(RUNTIME_FESVR_BUILD);					\
		$(RUNTIME_FESVR_SOURCE)/configure				\
			--srcdir=$(RUNTIME_FESVR_SOURCE)			\
			--prefix=$(RUNTIME_FESVR_INSTALL)

runtime-fesvr-build:
	@make -C $(RUNTIME_FESVR_BUILD) -j8 libfesvr.a libfesvr.so

runtime-fesvr-install:
	@mkdir -p $(RUNTIME_FESVR_INSTALL)/lib
	@cp -a $(RUNTIME_FESVR_BUILD)/libfesvr.* $(RUNTIME_FESVR_INSTALL)/lib
	@mkdir -p $(RUNTIME_FESVR_INSTALL)/include/fesvr
	@cp -a $(RUNTIME_FESVR_BUILD)/config.h $(RUNTIME_FESVR_INSTALL)/include/fesvr
	@cp -a $(RUNTIME_FESVR_SOURCE)/fesvr/*.h $(RUNTIME_FESVR_INSTALL)/include/fesvr
#	@make -C $(RUNTIME_FESVR_BUILD) install

runtime-fesvr-highfive:
	@echo "--- BUILD runtime-fesvr BEGIN ---"
	# 0. Remove old runtime-fesvr logfiles
	@test -d $(DIR_DADAO_LOG) || mkdir -p $(DIR_DADAO_LOG)
	@rm -fr $(RUNTIME_FESVR_LOG_STDOUT) $(RUNTIME_FESVR_LOG_STDERR)
	# 1. Clean old runtime-fesvr ...
	@make -s runtime-fesvr-clean				1>> $(RUNTIME_FESVR_LOG_STDOUT) 2>> $(RUNTIME_FESVR_LOG_STDERR)
	# 2. Clone runtime-fesvr ...
	@make -s runtime-fesvr-source				1>> $(RUNTIME_FESVR_LOG_STDOUT) 2>> $(RUNTIME_FESVR_LOG_STDERR)
	# 3. Configure runtime-fesvr ...
	@make -s runtime-fesvr-build-new			1>> $(RUNTIME_FESVR_LOG_STDOUT) 2>> $(RUNTIME_FESVR_LOG_STDERR)
	# 4. Make runtime-fesvr ...
	@make -s runtime-fesvr-build				1>> $(RUNTIME_FESVR_LOG_STDOUT) 2>> $(RUNTIME_FESVR_LOG_STDERR)
	# 5. Install runtime-fesvr ...
	@make -s runtime-fesvr-install				1>> $(RUNTIME_FESVR_LOG_STDOUT) 2>> $(RUNTIME_FESVR_LOG_STDERR)
	@echo "--- BUILD runtime-fesvr DONE! ---"

