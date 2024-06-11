#
# Makefile for binutils-gdb in toolchain
#
BINUTILS_GDB_0235_GITHUB		:= https://github.com/bminor/binutils-gdb.git
BINUTILS_GDB_0235_VERSION		:= binutils-2_35_2
BINUTILS_GDB_0235_BRANCH		:= dadao-0235
BINUTILS_GDB_0235_TARGET		?= dadao-unknown-elf

BINUTILS_GDB_0235_NEWFILES		:= $(DIR_DADAO_TOP)/TCH-binutils-gdb/binutils-gdb-0235-newfiles
BINUTILS_GDB_0235_PATCHES		:= $(DIR_DADAO_TOP)/TCH-binutils-gdb/binutils-gdb-0235-patches

BINUTILS_GDB_0235_SOURCE		:= $(DIR_DADAO_SOURCE)/binutils-gdb-0235
BINUTILS_GDB_0235_OPCODES		:= $(BINUTILS_GDB_0235_SOURCE)/opcodes/dadao-insn.opc
BINUTILS_GDB_0235_DISASSEMBLE		:= $(BINUTILS_GDB_0235_SOURCE)/opcodes/dadao-dis.h
BINUTILS_GDB_0235_ENCODING		:= $(BINUTILS_GDB_0235_SOURCE)/include/opcode/encoding.h
BINUTILS_GDB_0235_BUILD			:= $(DIR_DADAO_BUILD)/binutils-gdb-0235
BINUTILS_GDB_0235_LOG			:= $(DIR_DADAO_LOG)/binutils-gdb-0235.log
BINUTILS_GDB_0235_LOG_CHECK		:= $(DIR_DADAO_LOG)/binutils-gdb-0235.check

binutils-gdb-0235-clean:
	@rm -fr $(BINUTILS_GDB_0235_SOURCE)
	@rm -fr $(BINUTILS_GDB_0235_BUILD)

binutils-gdb-0235-source:
	@rm -fr $(BINUTILS_GDB_0235_SOURCE)
	# Clone remote repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(BINUTILS_GDB_0235_GITHUB) -- $(BINUTILS_GDB_0235_SOURCE)
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		git checkout -qb $(BINUTILS_GDB_0235_BRANCH) $(BINUTILS_GDB_0235_VERSION)
	# replace cru with crUu
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		grep -rl 'ARFLAGS = cru' ./ |				\
			xargs sed -i 's/ARFLAGS = cru/ARFLAGS = crUu/g';\
		git commit -asm"DADAO: remove ar warning messages"
	# handle include dir
	@cp -a $(BINUTILS_GDB_0235_NEWFILES)/include/* $(BINUTILS_GDB_0235_SOURCE)/include
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		git add include;					\
		git commit -sm"DADAO: add header files";		\
		git am $(BINUTILS_GDB_0235_PATCHES)/0*.patch
	# handle bfd dir
	@cp -a $(BINUTILS_GDB_0235_NEWFILES)/bfd/* $(BINUTILS_GDB_0235_SOURCE)/bfd
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		git add bfd;						\
		git commit -sm"DADAO: add bfd support";			\
		git am $(BINUTILS_GDB_0235_PATCHES)/1*.patch
	# BFD: re-generate configure and Makefile.in
	@cd $(BINUTILS_GDB_0235_SOURCE)/bfd;				\
		autoreconf2.69;						\
		git add .;						\
		git commit -sm"DADAO: re-generate bfd configure and Makefile.in"
	# BFD: re-generate bfd-in2.h and libbfd.h
	@cd $(BINUTILS_GDB_0235_SOURCE)/bfd;				\
		./configure;						\
		make headers;						\
		make distclean;						\
		git add .;						\
		git commit -sm"DADAO: re-generate bfd-in2.h and libbfd.h"
	# handle gas dir
	@cp -a $(BINUTILS_GDB_0235_NEWFILES)/gas/config/* $(BINUTILS_GDB_0235_SOURCE)/gas/config/
	@cp -a $(BINUTILS_GDB_0235_NEWFILES)/gas/testsuite/* $(BINUTILS_GDB_0235_SOURCE)/gas/testsuite/
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		git add gas/config;					\
		git commit -sm"DADAO: add gas config support";		\
		git add gas/testsuite;					\
		git commit -sm"DADAO: add gas testsuite support";	\
		git am $(BINUTILS_GDB_0235_PATCHES)/2*.patch
	# GAS: re-generate configure and Makefile.in
	@cd $(BINUTILS_GDB_0235_SOURCE)/gas;				\
		autoreconf2.69;						\
		git add .;						\
		git commit -sm"DADAO: re-generate gas configure and Makefile.in"
	# handle ld dir
	@cp -a $(BINUTILS_GDB_0235_NEWFILES)/ld/* $(BINUTILS_GDB_0235_SOURCE)/ld/
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		git add ld;						\
		git commit -sm"DADAO: add ld support";			\
		git am $(BINUTILS_GDB_0235_PATCHES)/3*.patch
	# LD: re-generate configure and Makefile.in
	@cd $(BINUTILS_GDB_0235_SOURCE)/ld;				\
		autoreconf2.69;						\
		git add .;						\
		git commit -sm"DADAO: re-generate ld configure and Makefile.in"
	# handle opcodes dir
	@cp -a $(BINUTILS_GDB_0235_NEWFILES)/opcodes/* $(BINUTILS_GDB_0235_SOURCE)/opcodes/
	# OPCODES: Generate dadao opcodes file
	@make -s -C $(DIR_DADAO_TOP) opcodes-binutils
	@make -s -C $(DIR_DADAO_TOP) opcodes-disassemble
	@make -s -C $(DIR_DADAO_TOP) opcodes-encoding
	@cp $(OPCODES_OUTPUT_BIN) $(BINUTILS_GDB_0235_OPCODES)
	@cp $(OPCODES_OUTPUT_DIS) $(BINUTILS_GDB_0235_DISASSEMBLE)
	@cp $(OPCODES_OUTPUT_ENV) $(BINUTILS_GDB_0235_ENCODING)
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		git add opcodes;					\
		git commit -sm"DADAO: add opcodes support";		\
		git am $(BINUTILS_GDB_0235_PATCHES)/4*.patch
	# OPCODES: re-generate configure and Makefile.in
	@cd $(BINUTILS_GDB_0235_SOURCE)/opcodes;			\
		autoreconf2.69;						\
		git add .;						\
		git commit -sm"DADAO: re-generate opcodes configure and Makefile.in"
	# handle binutils dir
	@cd $(BINUTILS_GDB_0235_SOURCE); git am $(BINUTILS_GDB_0235_PATCHES)/8*.patch
	# handle root dir
	@cd $(BINUTILS_GDB_0235_SOURCE); git am $(BINUTILS_GDB_0235_PATCHES)/9*.patch
	# CONFIG: re-generate configure and Makefile.in in root dir
	@cd $(BINUTILS_GDB_0235_SOURCE);				\
		autoreconf2.69;						\
		git add .;						\
		git commit -sm"DADAO: re-generate root configure"

binutils-gdb-0235-prepare:
	@rm -fr $(BINUTILS_GDB_0235_BUILD)
	@mkdir -p $(BINUTILS_GDB_0235_BUILD)
	@cd $(BINUTILS_GDB_0235_BUILD) &&				\
		$(BINUTILS_GDB_0235_SOURCE)/configure			\
			--disable-nls					\
			--target=$(BINUTILS_GDB_0235_TARGET)		\
			--srcdir=$(BINUTILS_GDB_0235_SOURCE)		\
			--prefix=$(DIR_DADAO_TCH_ELF)		\
			--disable-dependency-tracking

binutils-gdb-0235-build:
	@make $(__MAKE_J__) -C $(BINUTILS_GDB_0235_BUILD)

binutils-gdb-0235-install:
	@make $(__MAKE_J__) -C $(BINUTILS_GDB_0235_BUILD) install

binutils-gdb-0235-highfive:	dadao-before-highfive
	@test ! -f $(BINUTILS_GDB_0235_LOG) || mv --force $(BINUTILS_GDB_0235_LOG) $(BINUTILS_GDB_0235_LOG).last
	@echo "=== binutils-gdb-0235-highfive log file: $(BINUTILS_GDB_0235_LOG)"	| tee -a $(BINUTILS_GDB_0235_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"		| tee -a $(BINUTILS_GDB_0235_LOG)
	@make binutils-gdb-0235-clean							>> $(BINUTILS_GDB_0235_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"		| tee -a $(BINUTILS_GDB_0235_LOG)
	@make binutils-gdb-0235-source							>> $(BINUTILS_GDB_0235_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"		| tee -a $(BINUTILS_GDB_0235_LOG)
	@make binutils-gdb-0235-prepare							>> $(BINUTILS_GDB_0235_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"		| tee -a $(BINUTILS_GDB_0235_LOG)
	@make binutils-gdb-0235-build							>> $(BINUTILS_GDB_0235_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"		| tee -a $(BINUTILS_GDB_0235_LOG)
	@make binutils-gdb-0235-install							>> $(BINUTILS_GDB_0235_LOG) 2>&1
	@echo "--- binutils-gdb-0235-highfive DONE! ===         at `date +%T`"		| tee -a $(BINUTILS_GDB_0235_LOG)

binutils-gdb-0235-check:
	@rm -fr $(BINUTILS_GDB_0235_LOG_CHECK)
	@make -i -C $(BINUTILS_GDB_0235_BUILD) check-binutils	>> $(BINUTILS_GDB_0235_LOG_CHECK)
	@make -i -C $(BINUTILS_GDB_0235_BUILD) check-gas	>> $(BINUTILS_GDB_0235_LOG_CHECK)
	@make -i -C $(BINUTILS_GDB_0235_BUILD) check-ld		>> $(BINUTILS_GDB_0235_LOG_CHECK)
	@echo "RESULTS: binutils/binutils.sum gas/testsuite/gas.sum ld/ld.sum"

binutils-gdb-0235-TAGS:
	@make $(__MAKE_J__) -C $(BINUTILS_GDB_0235_BUILD) TAGS
	@etags -f $(BINUTILS_GDB_0235_SOURCE)/include/TAGS --recurse $(BINUTILS_GDB_0235_SOURCE)/include
	@etags -f $(BINUTILS_GDB_0235_SOURCE)/TAGS --etags-include=$(BINUTILS_GDB_0235_SOURCE)/include/TAGS
	@find $(BINUTILS_GDB_0235_BUILD) -name TAGS -exec		\
		etags -f $(BINUTILS_GDB_0235_SOURCE)/TAGS --append --etags-include='{}' \;

binutils-gdb-0235-tags:
	@cd $(BINUTILS_GDB_0235_SOURCE) &&				\
		ctags --recurse --exclude=testsuite --languages=C,C++	\
			./bfd ./binutils				\
			./gas ./gprof					\
			./include ./intl				\
			./ld ./libctf					\
			./opcodes					\
			./zlib
