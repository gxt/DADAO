#
# Makefile for simulator
#
QEMU_0801_GITLAB	:= https://gitlab.com/qemu-project/qemu.git
QEMU_0801_VERSION	:= v8.1.3
QEMU_0801_BRANCH	:= dadao-0801
QEMU_0801_TARGETS	:= dadao-softmmu,dadao-linux-user

QEMU_0801_NEWFILES	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0801-newfiles
QEMU_0801_PATCHES	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0801-patches
QEMU_0801_FIXUPS	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0801-fixups

QEMU_0801_SOURCE	:= $(DIR_DADAO_SOURCE)/qemu-0801
QEMU_0801_DECODEFILE	:= $(QEMU_0801_SOURCE)/target/dadao/insns.decode
QEMU_0801_DISASFILE	:= $(QEMU_0801_SOURCE)/disas/dadao.h
QEMU_0801_BUILD		:= $(DIR_DADAO_BUILD)/qemu-0801
QEMU_0801_INSTALL	?= $(DIR_DADAO_INSTALL)/__qemu-0801
QEMU_0801_LOG		:= $(DIR_DADAO_LOG)/qemu-0801.log

qemu-0801-clean:
	@rm -fr $(QEMU_0801_SOURCE)
	@rm -fr $(QEMU_0801_BUILD)
	@rm -fr $(QEMU_0801_INSTALL)

qemu-0801-source:
	@rm -fr $(QEMU_0801_SOURCE)
	# Clone source repo
	@$(__VAR_L__) git clone -q $(QEMU_0801_GITLAB) -- $(QEMU_0801_SOURCE)
	@cd $(QEMU_0801_SOURCE); git checkout -qb $(QEMU_0801_BRANCH) $(QEMU_0801_VERSION)
	# linux-user
	@cp -a $(QEMU_0801_NEWFILES)/linux-user/* $(QEMU_0801_SOURCE)/linux-user/
	@cd $(QEMU_0801_SOURCE);					\
		git add linux-user/dadao;				\
		git commit -sm"DADAO: add linux-user support"
	# target
	@cp -a $(QEMU_0801_NEWFILES)/target/* $(QEMU_0801_SOURCE)/target/
	@echo "TARGET-DECODE: Generate dadao decodetree file"
	@make -s -C $(DIR_DADAO_TOP) opcodes-qemu
	@cp $(OPCODES_OUTPUT_QEMU) $(QEMU_0801_DECODEFILE)
	@cd $(QEMU_0801_SOURCE);					\
		git add target/dadao;					\
		git commit -sm"DADAO: add target support"
	# disas
	@cp -a $(QEMU_0801_NEWFILES)/disas/* $(QEMU_0801_SOURCE)/disas/
	@make -s -C $(DIR_DADAO_TOP) opcodes-disassemble
	@cp $(OPCODES_OUTPUT_DIS) $(QEMU_0801_DISASFILE)
	@cd $(QEMU_0801_SOURCE);					\
		git add disas;						\
		git commit -sm"DADAO: add disas support"
	# hw
	@cp -a $(QEMU_0801_NEWFILES)/hw/* $(QEMU_0801_SOURCE)/hw/
	@cd $(QEMU_0801_SOURCE);					\
		git add hw/dadao;					\
		git commit -sm"DADAO: add hw support"
	# configs
	@cp -a $(QEMU_0801_NEWFILES)/configs/* $(QEMU_0801_SOURCE)/configs/
	@cd $(QEMU_0801_SOURCE);					\
		git add configs;					\
		git commit -sm"DADAO: add configs"
	# Patches
	@cd $(QEMU_0801_SOURCE); test ! -d $(QEMU_0801_PATCHES) || git am $(QEMU_0801_PATCHES)/*.patch
	# Fixups
	@cd $(QEMU_0801_SOURCE); test ! -d $(QEMU_0801_FIXUPS) || git am $(QEMU_0801_FIXUPS)/*.patch
	
qemu-0801-prepare:
	@rm -fr $(QEMU_0801_BUILD)
	@mkdir -p $(QEMU_0801_BUILD)
	@cd $(QEMU_0801_BUILD) &&								\
		$(__VAR_L__) $(QEMU_0801_SOURCE)/configure			\
			--target-list=$(QEMU_0801_TARGETS)				\
			--prefix=$(QEMU_0801_INSTALL)					\
			--extra-cflags=-I$(DIR_DADAO_TARGET)/include	\
			--disable-sdl					\
			--disable-capstone				\
			--disable-slirp					\
			--disable-fdt					\
			--enable-debug					\
			--enable-curses

qemu-0801-build:
	@make $(__MAKE_J__) -C $(QEMU_0801_BUILD)

qemu-0801-install:
	@rm -fr $(QEMU_0801_INSTALL)
	@make $(__MAKE_J__) -C $(QEMU_0801_BUILD) install
	@ln -sf -t $(DIR_DADAO_INSTALL)/bin $(QEMU_0801_INSTALL)/bin/*

qemu-0801-highfive:	dadao-before-highfive
	@test ! -f $(QEMU_0801_LOG) || mv --force $(QEMU_0801_LOG) $(QEMU_0801_LOG).last
	@echo "=== qemu-0801-highfive log file: $(QEMU_0801_LOG)"				| tee -a $(QEMU_0801_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(QEMU_0801_LOG)
	@make qemu-0801-clean									>> $(QEMU_0801_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(QEMU_0801_LOG)
	@make qemu-0801-source									>> $(QEMU_0801_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(QEMU_0801_LOG)
	@make qemu-0801-prepare									>> $(QEMU_0801_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(QEMU_0801_LOG)
	@make qemu-0801-build									>> $(QEMU_0801_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(QEMU_0801_LOG)
	@make qemu-0801-install									>> $(QEMU_0801_LOG) 2>&1
	@echo "--- qemu-0801-highfive DONE! ===                 at `date +%T`"	| tee -a $(QEMU_0801_LOG)

qemu-0801-tags:
	@make $(__MAKE_J__) -C $(QEMU_0801_SOURCE) ctags

