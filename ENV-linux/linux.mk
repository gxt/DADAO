#
# Makefile for linux-0504
#
LINUX_0504_GITHUB		:= https://github.com/torvalds/linux.git
LINUX_0504_VERSION		:= v5.4
# official download site :	https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/
LINUX_0504_VERSION_BASEURL	:= https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/
LINUX_0504_VERSION_LOCAL	:= /pub/linux-kernel-patches/
LINUX_0504_VERSION_PATCH	:= patch-5.4.100.xz
LINUX_0504_ARCH			:= dadao
LINUX_0504_BRANCH		:= dadao-0504
LINUX_0504_DEFCONFIG		:= dadao_defconfig

LINUX_0504_NEWFILES		:= $(DIR_DADAO_TOP)/ENV-linux/linux-0504-newfiles
LINUX_0504_PATCHES		:= $(DIR_DADAO_TOP)/ENV-linux/linux-0504-patches
LINUX_0504_FIXUPS		:= $(DIR_DADAO_TOP)/ENV-linux/linux-0504-fixups

LINUX_0504_SOURCE		:= $(DIR_DADAO_SOURCE)/linux-0504
LINUX_0504_BUILD		:= $(DIR_DADAO_BUILD)/linux-0504
LINUX_0504_TARGET		?= $(DIR_DADAO_TARGET)
LINUX_0504_LOG			:= $(DIR_DADAO_LOG)/linux-0504.log
LINUX_0504_HEADERS_LOG	:= $(DIR_DADAO_LOG)/linux-0504-headers.log

linux-0504-clean:
	# Remove old linux source dir ...
	@rm -fr $(LINUX_0504_SOURCE)
	# Remove old linux build dir ...
	@rm -fr $(LINUX_0504_BUILD)
	# Remove old linux header dir ...
	@rm -fr $(LINUX_0504_TARGET)/usr/include

linux-0504-source:
	# Remove old source dir
	@rm -fr $(LINUX_0504_SOURCE)
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(LINUX_0504_GITHUB) -- $(LINUX_0504_SOURCE)
	@cd $(LINUX_0504_SOURCE);						\
		git checkout -qb $(LINUX_0504_BRANCH) $(LINUX_0504_VERSION)
	# Add official patch
	@test -f $(LINUX_0504_VERSION_LOCAL)/$(LINUX_0504_VERSION_PATCH) ||	\
		wget --directory-prefix=$(LINUX_0504_VERSION_LOCAL)/		\
			$(LINUX_0504_VERSION_BASEURL)/$(LINUX_0504_VERSION_PATCH)
	@cd $(LINUX_0504_SOURCE);						\
		xzcat $(LINUX_0504_VERSION_LOCAL)/$(LINUX_0504_VERSION_PATCH)	\
			| patch -p1;						\
		git add .;							\
		git commit -sm"LINUX $(LINUX_0504_VERSION_PATCH)"
	# Add arch/dadao dir
	@ln --symbolic --target-directory=$(LINUX_0504_SOURCE)/arch/ $(LINUX_0504_NEWFILES)/arch/*
	@cd $(LINUX_0504_SOURCE);						\
		git add arch/dadao;						\
		git commit -sm"DADAO: add arch/dadao dir"
	# Apply linux-0504-patches ...
	@cd $(LINUX_0504_SOURCE); git am $(LINUX_0504_PATCHES)/*.patch
	# Apply linux-0504-fixups ...
	@cd $(LINUX_0504_SOURCE); git am $(LINUX_0504_FIXUPS)/*.patch

linux-0504-prepare:
	# Remove old linux build dir ...
	@rm -fr $(LINUX_0504_BUILD)
	# Make defconfig
	@mkdir -p $(LINUX_0504_BUILD)
	@make $(__MAKE_J__) -C $(LINUX_0504_SOURCE)	\
		O=$(LINUX_0504_BUILD)						\
		ARCH=$(LINUX_0504_ARCH)						\
		$(LINUX_0504_DEFCONFIG)

linux-0504-build:
	# Making (in several minutes) ...
	@make $(__MAKE_J__) -C $(LINUX_0504_BUILD)		\
		ARCH=$(LINUX_0504_ARCH)

linux-0504-install:
	# Make install ...
	@make $(__MAKE_J__) -C $(LINUX_0504_BUILD)		\
		ARCH=$(LINUX_0504_ARCH)						\
		INSTALL_PATH=$(LINUX_0504_TARGET)			\
		install

linux-0504-highfive:	dadao-before-highfive
	@test ! -f $(LINUX_0504_LOG) || mv --force $(LINUX_0504_LOG) $(LINUX_0504_LOG).last
	@echo "=== linux-0504-highfive log file: $(LINUX_0504_LOG)"				| tee -a $(LINUX_0504_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(LINUX_0504_LOG)
	@make linux-0504-clean									>> $(LINUX_0504_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(LINUX_0504_LOG)
	@make linux-0504-source									>> $(LINUX_0504_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(LINUX_0504_LOG)
	@make linux-0504-prepare								>> $(LINUX_0504_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(LINUX_0504_LOG)
#	@make linux-0504-build									>> $(LINUX_0504_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(LINUX_0504_LOG)
#	@make linux-0504-install								>> $(LINUX_0504_LOG) 2>&1
	@echo "--- linux-0504-highfive DONE! ===                at `date +%T`"	| tee -a $(LINUX_0504_LOG)

linux-0504-headers-check:
	# Headers check ...
	@make $(__MAKE_J__) -C $(LINUX_0504_BUILD)		\
		ARCH=$(LINUX_0504_ARCH)						\
		headers_check

linux-0504-headers-install:
	# Headers install ...
	@make $(__MAKE_J__) -C $(LINUX_0504_BUILD)		\
		ARCH=$(LINUX_0504_ARCH)						\
		INSTALL_HDR_PATH=$(DIR_DADAO_TCH_ELF)		\
		headers_install

linux-0504-headers-highfive:
	@test ! -f $(LINUX_0504_HEADERS_LOG) || mv --force $(LINUX_0504_HEADERS_LOG) $(LINUX_0504_HEADERS_LOG).last
	@echo "=== linux-0504-headers-highfive log file: $(LINUX_0504_HEADERS_LOG)"	| tee -a $(LINUX_0504_HEADERS_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"		| tee -a $(LINUX_0504_HEADERS_LOG)
	@rm -fr $(LINUX_0504_TARGET)/usr/include				>> $(LINUX_0504_HEADERS_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"		| tee -a $(LINUX_0504_HEADERS_LOG)
	@test -d $(LINUX_0504_SOURCE) || make linux-0504-source	>> $(LINUX_0504_HEADERS_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"		| tee -a $(LINUX_0504_HEADERS_LOG)
	@test -d $(LINUX_0504_BUILD) || make linux-0504-prepare	>> $(LINUX_0504_HEADERS_LOG) 2>&1
	@echo "--- 4. Check                                     at `date +%T`"		| tee -a $(LINUX_0504_HEADERS_LOG)
	@make linux-0504-headers-check							>> $(LINUX_0504_HEADERS_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"		| tee -a $(LINUX_0504_HEADERS_LOG)
	@make linux-0504-headers-install						>> $(LINUX_0504_HEADERS_LOG) 2>&1
	@echo "--- linux-0504-headers-highfive DONE! ===        at `date +%T`"		| tee -a $(LINUX_0504_HEADERS_LOG)
