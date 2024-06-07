#
# Makefile for newlib-cygwin in toolchain
#
NEWLIB_CYGWIN_0303_GITHUB	:= https://github.com/mirror/newlib-cygwin.git
NEWLIB_CYGWIN_0303_VERSION	:= newlib-3.3.0
NEWLIB_CYGWIN_0303_BRANCH	:= dadao-0303
NEWLIB_CYGWIN_0303_TARGET	?= dadao-unknown-elf

NEWLIB_CYGWIN_0303_NEWFILES	:= $(DIR_DADAO_TOP)/TCH-newlib-cygwin/newlib-cygwin-0303-newfiles
NEWLIB_CYGWIN_0303_PATCHES	:= $(DIR_DADAO_TOP)/TCH-newlib-cygwin/newlib-cygwin-0303-patches
NEWLIB_CYGWIN_0303_FIXUP	:= $(DIR_DADAO_TOP)/TCH-newlib-cygwin/newlib-cygwin-0303-fixup

NEWLIB_CYGWIN_0303_SOURCE	:= $(DIR_DADAO_SOURCE)/newlib-cygwin-0303
NEWLIB_CYGWIN_0303_BUILD	:= $(DIR_DADAO_BUILD)/newlib-cygwin-0303
NEWLIB_CYGWIN_0303_LOG		:= $(DIR_DADAO_LOG)/newlib-cygwin-0303.log

newlib-cygwin-0303-clean:
	@rm -fr $(NEWLIB_CYGWIN_0303_SOURCE)
	@rm -fr $(NEWLIB_CYGWIN_0303_BUILD)

newlib-cygwin-0303-source:
	@rm -fr $(NEWLIB_CYGWIN_0303_SOURCE)
	# Clone remote repo
	@$(__VAR_L__) git clone -q $(NEWLIB_CYGWIN_0303_GITHUB) -- $(NEWLIB_CYGWIN_0303_SOURCE)
	@cd $(NEWLIB_CYGWIN_0303_SOURCE); git checkout -qb $(NEWLIB_CYGWIN_0303_BRANCH) $(NEWLIB_CYGWIN_0303_VERSION)
	# include
	@echo "INCLUDE: Copy include dir from binutils-gdb (elf/dadao.h and opcode/dadao.h)"
	@cp -a $(DIR_DADAO_TOP)/TCH-binutils-gdb/binutils-gdb-0235-newfiles/include/* $(NEWLIB_CYGWIN_0303_SOURCE)/include
	@cd $(NEWLIB_CYGWIN_0303_SOURCE);					\
		git add include;						\
		git commit -sm"DADAO-include: add header files"
	@echo "INCLUDE: Apply include related patches ..."
	@cd $(NEWLIB_CYGWIN_0303_SOURCE); git am $(NEWLIB_CYGWIN_0303_PATCHES)/0*.patch
	# newlib/libc/sys/dadao
	@echo "NEWLIB-LIBC-SYS-DADAO: Copy newlib/libc/sys/dadao dir"
	@cp -a $(NEWLIB_CYGWIN_0303_NEWFILES)/newlib/libc/sys/dadao $(NEWLIB_CYGWIN_0303_SOURCE)/newlib/libc/sys
	@cd $(NEWLIB_CYGWIN_0303_SOURCE);					\
		git add newlib/libc/sys/dadao;					\
		git commit -sm"DADAO-newlib-libc-sys-dadao: add dadao support"
	@echo "NEWLIB-LIBC-SYS-DADAO: configure (FIXME)"
	@cd $(NEWLIB_CYGWIN_0303_SOURCE)/newlib/libc/sys/dadao;			\
		cp ../mmixware/configure .;					\
		git add .;							\
		git commit -sm"DADAO-newlib-libc-sys-dadao: copy a workable configure"
	@echo "NEWLIB-LIBC-SYS-DADAO: Makefile.in (FIXME)"
	@cd $(NEWLIB_CYGWIN_0303_SOURCE)/newlib/libc/sys/dadao;			\
		cp ../mmixware/Makefile.in .;					\
		git add .;							\
		git commit -sm"DADAO-newlib-libc-sys-dadao: copy a workable Makefile.in"
#	@cd $(NEWLIB_CYGWIN_0303_SOURCE)/newlib/libc/sys/dadao;			\
#		autoconf2.64;							\
#		git add .;							\
#		git commit -sm"DADAO-newlib-libc-sys-dadao: re-generate configure"
	# newlib/libc/sys
	@echo "NEWLIB-LIBC-SYS: Apply config related patches ..."
	@cd $(NEWLIB_CYGWIN_0303_SOURCE); git am $(NEWLIB_CYGWIN_0303_PATCHES)/1*.patch
	@echo "NEWLIB-LIBC-SYS: re-generate configure"
	@cd $(NEWLIB_CYGWIN_0303_SOURCE)/newlib/libc/sys;			\
		autoconf2.64;							\
		git add .;							\
		git commit -sm"DADAO-newlib-libc-sys: re-generate configure"
	# newlib
	@echo "NEWLIB: Apply config related patches ..."
	@cd $(NEWLIB_CYGWIN_0303_SOURCE); git am $(NEWLIB_CYGWIN_0303_PATCHES)/2*.patch
	# root config
	@echo "CONFIG: Apply config related patches ..."
	@cd $(NEWLIB_CYGWIN_0303_SOURCE); git am $(NEWLIB_CYGWIN_0303_PATCHES)/9*.patch
	@echo "CONFIG: re-generate configure in root dir (NOTE: require exactly autoconf 2.64)"
	@cd $(NEWLIB_CYGWIN_0303_SOURCE);					\
		autoreconf2.64;							\
		git add .;							\
		git commit -sm"DADAO: re-generate root configure"
	# fixups
	#@echo "FIXUP: Apply fixup ..."
	#@cd $(NEWLIB_CYGWIN_0303_SOURCE); git am $(NEWLIB_CYGWIN_0303_FIXUP)/*.patch

newlib-cygwin-0303-prepare:
	@rm -fr $(NEWLIB_CYGWIN_0303_BUILD)
	@mkdir -p $(NEWLIB_CYGWIN_0303_BUILD)
	# CFLAGS_FOR_TARGET="-g -Os" can be added before configure to modify default CFLAGS
	@cd $(NEWLIB_CYGWIN_0303_BUILD) &&						\
		$(__VAR_P__) $(NEWLIB_CYGWIN_0303_SOURCE)/configure	\
			--target=$(NEWLIB_CYGWIN_0303_TARGET)			\
			--srcdir=$(NEWLIB_CYGWIN_0303_SOURCE)			\
			--prefix=$(DIR_DADAO_TCH_ELF)					\
			--with-build-time-tools=$(DIR_DADAO_INSTALL)/bin

newlib-cygwin-0303-build:
	@make $(__VAR_P__) $(__MAKE_J__) -C $(NEWLIB_CYGWIN_0303_BUILD) all

newlib-cygwin-0303-install:
	@make $(__VAR_P__) $(__MAKE_J__) -C $(NEWLIB_CYGWIN_0303_BUILD) install

newlib-cygwin-0303-highfive:	dadao-before-highfive
	@test ! -f $(NEWLIB_CYGWIN_0303_LOG) || mv --force $(NEWLIB_CYGWIN_0303_LOG) $(NEWLIB_CYGWIN_0303_LOG).last
	@echo "=== newlib-cygwin-0303-highfive log file: $(NEWLIB_CYGWIN_0303_LOG)"	| tee -a $(NEWLIB_CYGWIN_0303_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"		| tee -a $(NEWLIB_CYGWIN_0303_LOG)
	@make newlib-cygwin-0303-clean							>> $(NEWLIB_CYGWIN_0303_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"		| tee -a $(NEWLIB_CYGWIN_0303_LOG)
	@make newlib-cygwin-0303-source							>> $(NEWLIB_CYGWIN_0303_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"		| tee -a $(NEWLIB_CYGWIN_0303_LOG)
	@make newlib-cygwin-0303-prepare						>> $(NEWLIB_CYGWIN_0303_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"		| tee -a $(NEWLIB_CYGWIN_0303_LOG)
	@make newlib-cygwin-0303-build							>> $(NEWLIB_CYGWIN_0303_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"		| tee -a $(NEWLIB_CYGWIN_0303_LOG)
	@make newlib-cygwin-0303-install						>> $(NEWLIB_CYGWIN_0303_LOG) 2>&1
	@echo "--- newlib-cygwin-0303-highfive DONE! ===        at `date +%T`"		| tee -a $(NEWLIB_CYGWIN_0303_LOG)
