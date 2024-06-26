#
# Makefile for gcc in toolchain
#
GCC_1003_GITHUB		:= https://github.com/gcc-mirror/gcc.git
GCC_1003_VERSION	:= releases/gcc-10.3.0
GCC_1003_BRANCH		:= dadao-1003
GCC_1003_TARGET		?= dadao-unknown-elf

GCC_1003_NEWFILES	:= $(DIR_DADAO_TOP)/TCH-gcc/gcc-1003-newfiles
GCC_1003_PATCHES	:= $(DIR_DADAO_TOP)/TCH-gcc/gcc-1003-patches
GCC_1003_FIXUP		:= $(DIR_DADAO_TOP)/TCH-gcc/gcc-1003-fixups

GCC_1003_SOURCE		:= $(DIR_DADAO_SOURCE)/gcc-1003
GCC_1003_BUILD		:= $(DIR_DADAO_BUILD)/gcc-1003
GCC_1003_LOG		:= $(DIR_DADAO_LOG)/gcc-1003.log

gcc-1003-clean:
	@rm -fr $(GCC_1003_SOURCE)
	@rm -fr $(GCC_1003_BUILD)

gcc-1003-source:
	@rm -fr $(GCC_1003_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(GCC_1003_GITHUB) -- $(GCC_1003_SOURCE)
	@cd $(GCC_1003_SOURCE); git checkout -qb $(GCC_1003_BRANCH) $(GCC_1003_VERSION)
	# gcc/config
	@cp -a $(GCC_1003_NEWFILES)/gcc/config/* $(GCC_1003_SOURCE)/gcc/config/
	@cd $(GCC_1003_SOURCE);						\
		git add gcc/config/dadao;				\
		git commit -sm"DADAO: add gcc/config/dadao support"
	# gcc/common/config
	@cp -a $(GCC_1003_NEWFILES)/gcc/common/config/* $(GCC_1003_SOURCE)/gcc/common/config/
	@cd $(GCC_1003_SOURCE);						\
		git add gcc/common/config/dadao;			\
		git commit -sm"DADAO: add gcc/common/config/dadao support"
	# gcc dir patches
	@cd $(GCC_1003_SOURCE); git am $(GCC_1003_PATCHES)/1*.patch
	# gcc dir fixups
	@cd $(GCC_1003_SOURCE); git am $(GCC_1003_FIXUP)/0*.patch
	# libgcc/config
	@cp -a $(GCC_1003_NEWFILES)/libgcc/config/* $(GCC_1003_SOURCE)/libgcc/config/
	@cd $(GCC_1003_SOURCE);						\
		git add libgcc/config/dadao;				\
		git commit -sm"DADAO: add libgcc/config/dadao support"
	# libgcc dir patches
	@cd $(GCC_1003_SOURCE); git am $(GCC_1003_PATCHES)/2*.patch
	# root config
	@cd $(GCC_1003_SOURCE); git am $(GCC_1003_PATCHES)/9*.patch
	# re-generate configure and Makefile.in in root dir
	@cd $(GCC_1003_SOURCE);						\
		autoreconf2.69;						\
		git add .;						\
		git commit -sm"DADAO: re-generate root configure"

gcc-1003-prepare:
	@rm -fr $(GCC_1003_BUILD)
	@mkdir -p $(GCC_1003_BUILD)
	# FIXME: add -Wno-format to remove a heap of compiling warnings
	@cd $(GCC_1003_BUILD) &&					\
		CFLAGS="-Wno-format -g -O2"				\
		CPPFLAGS="-Wno-format -g -O2"				\
		CXXFLAGS="-Wno-format -g -O2"				\
		$(GCC_1003_SOURCE)/configure				\
			--target=$(GCC_1003_TARGET)			\
			--srcdir=$(GCC_1003_SOURCE)			\
			--prefix=$(DIR_DADAO_TCH_ELF)		\
			--disable-libssp				\
			--disable-libcilkrts				\
			--enable-languages=c				\
			--disable-shared				\
			--disable-threads				\
			--disable-libatomic				\
			--disable-decimal-float				\
			--disable-libffi				\
			--disable-libgomp				\
			--disable-libitm				\
			--disable-libmpx				\
			--disable-libquadmath				\
			--disable-libsanitizer				\
			--with-newlib

gcc-1003-build:
	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(GCC_1003_BUILD) all
#	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(GCC_1003_BUILD) all-gcc		; if ONLY gcc required, no libgcc and others

gcc-1003-install:
	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(GCC_1003_BUILD) install
#	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(GCC_1003_BUILD) install-gcc		; if ONLY gcc required, no libgcc and others

gcc-1003-highfive:	dadao-before-highfive
	@test ! -f $(GCC_1003_LOG) || mv --force $(GCC_1003_LOG) $(GCC_1003_LOG).last
	@echo "=== gcc-1003-highfive log file: $(GCC_1003_LOG)"					| tee -a $(GCC_1003_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(GCC_1003_LOG)
	@make gcc-1003-clean									>> $(GCC_1003_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(GCC_1003_LOG)
	@make gcc-1003-source									>> $(GCC_1003_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(GCC_1003_LOG)
	@make gcc-1003-prepare									>> $(GCC_1003_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(GCC_1003_LOG)
	@make gcc-1003-build									>> $(GCC_1003_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(GCC_1003_LOG)
	@make gcc-1003-install									>> $(GCC_1003_LOG) 2>&1
	@echo "--- gcc-1003-highfive DONE! ===                  at `date +%T`"	| tee -a $(GCC_1003_LOG)

gcc-1003-check:
	@echo "Run gcc-testsuite with qemu simulator"
	@test -f $(DIR_DADAO_INSTALL)/usr/bin/runtest || make $(__VAR_DADAO_PATH__) -C $(DIR_DADAO_TOP) runtime-qemu-dejagnu-prepare
	@make $(__VAR_DADAO_PATH__) $(__MAKE_J__) -C $(GCC_1003_BUILD) check-gcc-c				\
		RUNTESTFLAGS="--target_board=qemu-dadao"

gcc-1003-tags:
	@cd $(GCC_1003_SOURCE) &&							\
		ctags --recurse --exclude=testsuite --languages=C,C++			\
			./fixincludes							\
			./gcc $(GCC_1003_BUILD)/gcc					\
			./include ./intl						\
			./libbacktrace ./libcc1 ./libcpp ./libdecnumber ./libiberty	\
			./libgcc ./libquadmath						\
			./lto-plugin							\
			./zlib
